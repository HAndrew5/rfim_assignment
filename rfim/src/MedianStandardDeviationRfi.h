#ifndef INCLUDE_RFIM_MEDIAN_STANDARD_DEVIATION_RFI
#define INCLUDE_RFIM_MEDIAN_STANDARD_DEVIATION_RFI

#include<algorithm>
#include<cassert>

#include"TimeFrequency.h"
#include"RfiStrategy.h"

namespace rfim {

	/*
	This class is based on the method used in "rfi_clean.cpp".
	It calculates the standard deviation from the median and sets all samples to the median for every 
	channel containing a sample greater than some threshold number of standard deviations above the median.
	It has only been re-written to improve performance.
	No parallelism has been used.
	The median is computed using the standard library nth_element.
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

		MedianStandardDeviationRfi(DataType threshold = MedianStandardDeviationRfi<DataType>::default_threshold()) :
			_threshold(threshold)
		{}

		void processImpl(TimeFrequency<DataType>& data_buffer)
		{
			std::size_t median_offset = data_buffer.get_number_of_spectra() / 2;
			assert(median_offset > 2);
			TimeFrequency<DataType> temp_buffer(data_buffer); // if memory is a concern, can just have 1 channels worth of temp and copy in loop
			
			std::size_t n_flagged_channels = 0;

			for (std::size_t i_channel =0; i_channel<data_buffer.get_number_of_channels(); ++i_channel)
			{
				// median
				DataType* start_pointer = temp_buffer.get_raw_channel_start(i_channel);
				DataType* median_pointer = temp_buffer.get_raw_channel_start(i_channel) + median_offset;
				DataType* end_pointer = temp_buffer.get_raw_channel_start(i_channel) + temp_buffer.get_number_of_spectra() - 1;
				std::nth_element(start_pointer, median_pointer, end_pointer);
				DataType median = *median_pointer;

				// STD
				DataType square_sum = 0;
				for (std::size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
				{
					DataType d = data_buffer.get_sample(i_channel, i_sample) - median;
					square_sum += d * d;
				}
				DataType standard_deviation = std::sqrt(square_sum / data_buffer.get_number_of_spectra());

				// Clean Data
				DataType n_std = _threshold * standard_deviation;
				for (std::size_t i_sample = 0; i_sample < data_buffer.get_number_of_spectra(); ++i_sample)
				{
					if (data_buffer.get_sample(i_channel, i_sample) > n_std + median)
					{
						n_flagged_channels++;
						for (std::size_t j_sample = 0; j_sample < data_buffer.get_number_of_spectra(); ++j_sample)
							data_buffer.get_sample(i_channel, j_sample) = median;
						break;
					}
				}

				std::cout << "Processed channel: " << i_channel << " \r";
			}
			std::cout << "channel flagged: " << n_flagged_channels << "\n";
		}

	private:
		DataType _threshold;

		static DataType default_threshold()
		{
			if (std::is_floating_point<DataType>::value)
				return static_cast<DataType>(4.5);
			else if (std::is_integral<DataType>::value)
				return static_cast<DataType>(5);
			return DataType();
		}
	};

} // namespace: rfim
#endif