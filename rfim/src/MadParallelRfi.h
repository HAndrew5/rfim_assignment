#ifndef INCLUDE_RFIM_MAD_PARALLEL_RFI
#define INCLUDE_RFIM_MAD_PARALLEL_RFI

#include<algorithm>
#include<cassert>
#include<vector>

#include<omp.h>

#include"TimeFrequency.h"
#include"RfiStrategy.h"

namespace rfim {

	/*
	This class implements the RfiStrategy CRTP interface.
	It is based on the median absolute distribution (MAD).
	The MAD is the median of the absolute deviations from the datas median.
	i.e 
	* calculate the median
	* find how far each sample is away from the median
	* calculate the median of this
	The detection threshold can be set with a constructor argument.
	The loop over channels has been parallelised with OMP.
	*/
	template<typename DataType>
	class MadParallelRfi : public RfiStrategy<MadParallelRfi<DataType>>
	{
		static_assert(
			std::is_same<DataType, float>::value ||
			std::is_same<DataType, uint8_t>::value ||
			std::is_same<DataType, uint16_t>::value,
			"MadParallelRfi DataType must be float, uint8_t, or uint16_t"
			);

	public:
		using StrategyDataType = DataType;

		MadParallelRfi(TimeFrequencyMetadata metadata, float threshold = 10.0f) :
			_threshold(threshold),
			_temp_buffer(metadata),
			_median_offset(_temp_buffer.get_number_of_spectra() / 2)
		{}

		size_t process_impl(TimeFrequency<DataType>& data_buffer)
		{
			data_buffer.write_data_to_time_frequency(_temp_buffer);
			size_t n_flagged_channels = 0;
			int endChannels = static_cast<int>(data_buffer.get_number_of_channels());

#pragma omp parallel for reduction(+:n_flagged_channels)
			for (int i_channel = 0; i_channel < endChannels; ++i_channel)
			{
				DataType median = _temp_buffer.destructive_calculate_channel_median(i_channel);
				DataType mad = calculate_mad(data_buffer, i_channel, median);

				DataType rfi_threshold = static_cast<DataType>(mad * _threshold) + median;
				if (does_channel_contain_rfi(data_buffer, i_channel, rfi_threshold))
				{
					n_flagged_channels++;
					data_buffer.set_channel_to_value(i_channel, median);
				}
			}
			return n_flagged_channels;
		}

		bool does_channel_contain_rfi(const TimeFrequency<DataType>& data_buffer, ChannelCount channel, DataType rfi_threshold) const
		{
			for (size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
			{
				if (data_buffer.get_sample(channel, i_sample) > rfi_threshold)
					return true;
			}
			return false;
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_integral<T>::value, DataType>::type
		calculate_mad(TimeFrequency<DataType>& data_buffer, size_t i_channel, DataType median)
		{
			std::vector<DataType> _median_deviations(data_buffer.get_number_of_spectra());
			for (size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
			{
				_median_deviations[i_sample] = data_buffer.get_sample(i_channel, i_sample) > median ?
					(data_buffer.get_sample(i_channel, i_sample) - median) :
					(median - data_buffer.get_sample(i_channel, i_sample));
			}

			std::nth_element(_median_deviations.begin(), _median_deviations.begin() + _median_offset, _median_deviations.end());
			DataType mad = _median_deviations[_median_offset];
			if( mad > 0)
				return mad;
			return static_cast<DataType>(1);
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_floating_point<T>::value, float>::type
		calculate_mad(TimeFrequency<DataType>& data_buffer, size_t i_channel, DataType median)
		{
			std::vector<DataType> _median_deviations(data_buffer.get_number_of_spectra());
			for (size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
				_median_deviations[i_sample] = std::abs(data_buffer.get_sample(i_channel, i_sample) - median);

			std::nth_element(_median_deviations.begin(), _median_deviations.begin() + _median_offset, _median_deviations.end());
			float mad = _median_deviations[_median_offset];
			if (mad > 0.0f)
				return mad;
			return 1e-6f;
		}

		float get_threshold() const
		{
			return _threshold;
		}

	private:
		float _threshold;
		TimeFrequency<DataType> _temp_buffer;
		size_t _median_offset;
	};

} // namespace: rfim
#endif