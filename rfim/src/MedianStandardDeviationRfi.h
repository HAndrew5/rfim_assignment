#ifndef INCLUDE_RFIM_MEDIAN_STANDARD_DEVIATION_RFI
#define INCLUDE_RFIM_MEDIAN_STANDARD_DEVIATION_RFI

#include"TimeFrequency.h"
#include"RfiStrategy.h"

namespace rfim {

	/*
	This class implements the RfiStrategy CRTP interface.
	It is based on the method used in "rfi_clean.cpp".
	It calculates the standard deviation from the median and sets all samples to the median for every 
	channel containing a sample greater than some threshold number of standard deviations above the median.
	The detection threshold can be set with a constructor argument.
	*/
	template<typename DataType>
	class MedianStandardDeviationRfi : public RfiStrategy<MedianStandardDeviationRfi<DataType>>
	{
		static_assert(
			std::is_same<DataType, float>::value ||
			std::is_same<DataType, uint8_t>::value ||
			std::is_same<DataType, uint16_t>::value,
			"MedianStandardDeviationRfi DataType must be float, uint8_t, or uint16_t"
			);

	public:
		using StrategyDataType = DataType;

		MedianStandardDeviationRfi(TimeFrequencyMetadata metadata, float threshold = 4.5) :
			_threshold(threshold),
			_temp_buffer(metadata)
		{}

		size_t process_impl(TimeFrequency<DataType>& data_buffer)
		{
			data_buffer.write_data_to_time_frequency(_temp_buffer); // if memory is a concern, can just have 1 channels worth of temp and copy in loop
			size_t n_flagged_channels = 0;

			for (size_t i_channel =0; i_channel<data_buffer.get_number_of_channels(); ++i_channel)
			{
				DataType median = _temp_buffer.destructive_calculate_channel_median(i_channel);
				if (does_channel_contain_rfi(data_buffer, i_channel, median))
				{
					n_flagged_channels++;
					data_buffer.set_channel_to_value(i_channel, median);
				}
			}
			return n_flagged_channels;
		}
		
		bool does_channel_contain_rfi(const TimeFrequency<DataType>& data_buffer, ChannelCount channel, DataType median) const
		{
			float standard_deviation = data_buffer.calculate_channel_standard_deviation(channel, median);
			DataType rfi_threshold = static_cast<DataType>(_threshold * standard_deviation) + median;
			for (size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
			{
				if (data_buffer.get_sample(channel, i_sample) > rfi_threshold)
					return true;
			}
			return false;
		}

		float get_threshold() const
		{
			return _threshold;
		}


	private:
		float _threshold;
		TimeFrequency<DataType> _temp_buffer;
	};

} // namespace: rfim
#endif