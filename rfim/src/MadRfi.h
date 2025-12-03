#ifndef INCLUDE_RFIM_MAD_RFI
#define INCLUDE_RFIM_MAD_RFI

#include<algorithm>
#include<cassert>
#include<vector>

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
	*/
	template<typename DataType>
	class MadRfi : public RfiStrategy<MadRfi<DataType>>
	{
		static_assert(
			std::is_same<DataType, float>::value ||
			std::is_same<DataType, uint8_t>::value ||
			std::is_same<DataType, uint16_t>::value,
			"MadRfi DataType must be float, uint8_t, or uint16_t"
			);

	public:
		using StrategyDataType = DataType;

		MadRfi(TimeFrequencyMetadata metadata, float threshold = 4.5f) :
			_threshold(threshold),
			_temp_buffer(metadata),
			_median_offset(_temp_buffer.get_number_of_spectra() / 2),
			_median_deviations(_temp_buffer.get_number_of_spectra())
		{}

		size_t processImpl(TimeFrequency<DataType>& data_buffer)
		{
			data_buffer.write_data_to_time_frequency(_temp_buffer);
			std::size_t n_flagged_channels = 0;

			for (std::size_t i_channel = 0; i_channel < data_buffer.get_number_of_channels(); ++i_channel)
			{
				DataType median = _temp_buffer.destructive_calculate_channel_median(i_channel);
				DataType mad = calculate_mad(data_buffer, i_channel, median);

				DataType value_threshold = mad * static_cast<DataType>(_threshold) + median;
				if (does_channel_contain_rfi(data_buffer, i_channel, value_threshold))
				{
					n_flagged_channels++;
					data_buffer.set_channel_to_value(i_channel, median);
				}
			}
			return n_flagged_channels;
		}

		bool does_channel_contain_rfi(const TimeFrequency<DataType>& data_buffer, ChannelCount channel, DataType value_threshold) const
		{
			for (size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
			{
				if (data_buffer.get_sample(channel, i_sample) > value_threshold)
					return true;
			}
			return false;
		}

		DataType calculate_mad(TimeFrequency<DataType>& data_buffer, size_t i_channel, DataType median)
		{
			for (std::size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
				_median_deviations[i_sample] = std::abs(data_buffer.get_sample(i_channel, i_sample) - median);

			std::nth_element(_median_deviations.begin(), _median_deviations.begin() + _median_offset, _median_deviations.end());
			DataType mad = _median_deviations[_median_offset];
			if( mad > 0)
				return mad;
			return static_cast<DataType>(1);
		}

		float get_threshold() const
		{
			return _threshold;
		}

	private:
		float _threshold;
		TimeFrequency<DataType> _temp_buffer;
		size_t _median_offset;
		std::vector<DataType> _median_deviations;
	};

} // namespace: rfim
#endif