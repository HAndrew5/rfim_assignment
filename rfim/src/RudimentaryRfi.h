#ifndef INCLUDE_RFIM_RUDIMENTARY_RFI
#define INCLUDE_RFIM_RUDIMENTARY_RFI

#include<vector>

#include"TimeFrequency.h"
#include"RfiStrategy.h"

namespace rfim {

	/*
	This class is based on the functions provided in the example "rfi_clean.cpp".
	It has only been modified a minimum amount to work with the TimeFrequency
	class and the RfiStrategy CRPT.
	This serves as a baseline, but is not intended as an example of "clean" or
	efficient code.
	NOTE: This has NOT been changed to work with uint types, it is only valid for floats,
	and has not been unit tested. Please look to MedianStandardDeviationRfi for a similar 
	strategy that is fully functional and more optimised.
	*/
	template<typename DataType>
	class RudimentaryRfi : public RfiStrategy<RudimentaryRfi<DataType>>
	{
		static_assert(
			std::is_same<DataType, float>::value,
			"RudimentaryRfi DataType must be float"
			);

	public:
		using StrategyDataType = DataType;

		RudimentaryRfi(DataType threshold = RudimentaryRfi<DataType>::default_threshold()) :
			_threshold(threshold)
		{}

		size_t process_impl(TimeFrequency<DataType>& data_buffer)
		{
			std::cout << "[RudimentaryRfi] Processing...\n";
			std::vector<DataType> median(data_buffer.get_number_of_channels());
			std::vector<DataType> std_dev(data_buffer.get_number_of_channels());

			calculate_median(data_buffer, median);
			calculate_std(data_buffer, median, std_dev);
			size_t n_cleaned_channels = clean_data(data_buffer, median, std_dev);
			return n_cleaned_channels;
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

		void calculate_median(TimeFrequency<DataType> chunk, std::vector<DataType>& median)
		{
			DataType max_value;
			if (std::is_floating_point<DataType>::value)
				max_value = static_cast<DataType>(1e7);
			else if (std::is_same<DataType, uint8_t>::value)
				max_value = static_cast<DataType>(255);
			else if (std::is_same<DataType, uint16_t>::value)
				max_value = static_cast<DataType>(65535);

			for (unsigned channel = 0; channel < chunk.get_number_of_channels(); ++channel)
			{
				std::vector<DataType> temp(chunk.get_number_of_spectra());

				unsigned sample = 0;
				std::cout << "calculating median for channel: " << channel << " \r";
				for (unsigned iter = 0; iter < temp.size() / 2 + 1; ++iter)
				{

					float min = max_value;
					unsigned arg = 0;
					for (unsigned i = 0; i < temp.size(); ++i)
					{
						if (min > chunk.get_sample(channel, i))
						{
							min = chunk.get_sample(channel, i);
							arg = i;
						}
					}
					temp[iter] = min;
					chunk.get_sample(channel, arg) = max_value;
				}
				median[channel] = (temp[temp.size() / 2] + temp[temp.size() / 2 - 1]) / 2.0;
			}
			std::cout << "\n done estimating median for the chunk \n";
		}

		void calculate_std(TimeFrequency<DataType>& chunk, std::vector<DataType>& median, std::vector<DataType>& std_dev)
		{
			double sum;
			for (unsigned int channel = 0; channel < chunk.get_number_of_channels(); ++channel)
			{
				sum = 0.0;

				for (unsigned sample = 0; sample < chunk.get_number_of_spectra(); ++sample)
				{
					sum += std::pow(chunk.get_sample(channel, sample) - median[channel], 2);
				}
				std_dev[channel] = std::sqrt(sum / static_cast<double>(chunk.get_number_of_spectra()));
			}
		}

		size_t clean_data(TimeFrequency<DataType>& chunk, std::vector<DataType>& median, std::vector<DataType>& std_dev)
		{
			size_t number_of_channels_flagged = 0;
			for (unsigned channel = 0; channel < chunk.get_number_of_channels(); ++channel)
			{
				int flag = 0;
				for (unsigned int sample = 0; sample < chunk.get_number_of_spectra(); ++sample)
				{
					if (chunk.get_sample(channel, sample) > (_threshold * std_dev[channel] + median[channel]))
					{
						flag = 1;
					}
				}
				if (flag == 1)
				{
					number_of_channels_flagged++;
					for (unsigned int sample = 0; sample < chunk.get_number_of_spectra(); ++sample) chunk.get_sample(channel, sample) = median[channel];
				}

			}
			std::cout << "channel flagged: " << number_of_channels_flagged << "\n";
			return number_of_channels_flagged;
		}
	};

} // namespace: rfim
#endif