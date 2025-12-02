#ifndef INCLUDE_RFIM_TIME_FREQUENCY
#define INCLUDE_RFIM_TIME_FREQUENCY

#include<string>
#include<type_traits>
#include<cmath>
#include<cassert>
#include<stdexcept>
#include<algorithm>

#include"TimeFrequencyMetadata.h"

namespace rfim {

	template <typename DataType>
	class TimeFrequency
	{
		static_assert(
			std::is_same<DataType, float>::value ||
			std::is_same<DataType, uint8_t>::value ||
			std::is_same<DataType, uint16_t>::value,
			"TimeFrequency DataType must be float, uint8_t, or uint16_t"
			);

	public:
		TimeFrequency(TimeFrequencyMetadata initialisation_info) :
			_metadata(initialisation_info),
			_data(new DataType[_metadata._number_of_spectra * _metadata._frequency_channels]() )
		{
		}

		TimeFrequency(const TimeFrequency& input_data) :
			_metadata(input_data._metadata),
			_data(new DataType[_metadata._number_of_spectra * _metadata._frequency_channels]())
		{
			std::copy(input_data._data, input_data._data + input_data.get_total_samples(), _data);
		}

		~TimeFrequency()
		{
			delete[] _data;
		}

		void read_data_from_raw(const DataType* source_buffer)
		{
			if(!source_buffer)
			{
				std::string error_string = "Null pointer passed when trying to read raw data in rfim::TimeFrequency.read_data_from_raw";
				throw std::invalid_argument(error_string);
			}

			std::copy(source_buffer, source_buffer + get_total_samples(), _data);
		}

		void write_data_to_raw(DataType* destination_buffer) const
		{
			if (!destination_buffer)
			{
				std::string error_string = "Null pointer passed when trying to write raw data in rfim::TimeFrequency.write_data_to_raw";
				throw std::invalid_argument(error_string);
			}

			std::copy(_data, _data + get_total_samples(), destination_buffer);
		}

		void write_data_to_time_frequency(TimeFrequency<DataType>& destination_buffer) const
		{
			if (destination_buffer.get_total_samples() != get_total_samples())
			{
				std::string error_string = "Tried writing from a TimeFrequency buffer of length " +
					std::to_string(get_total_samples()) + " to a TimeFrequency of length " + 
					std::to_string(destination_buffer.get_total_samples()) + 
						", these values must match in rfim::TimeFrequency.write_data_to_time_frequency";
				throw std::out_of_range(error_string);
			}

			std::copy(_data, _data + get_total_samples(), destination_buffer._data);
		}

		void set_channel_to_value(ChannelCount channel, DataType value)
		{
			DataType* start_pointer = get_raw_channel_start(channel);
			std::fill(start_pointer, start_pointer+get_number_of_spectra(), value);
		}

		DataType destructive_calculate_channel_median(ChannelCount channel)
		{
			DataType* start_pointer = get_raw_channel_start(channel);
			DataType* median_pointer = get_raw_channel_start(channel) + get_number_of_spectra() / 2;
			DataType* end_pointer = get_raw_channel_start(channel) + get_number_of_spectra();
			std::nth_element(start_pointer, median_pointer, end_pointer);
			return *median_pointer;
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_integral<T>::value, float>::type
		calculate_channel_standard_deviation(ChannelCount channel_index, float channel_average) const
		{
			// Calculate using floating point to avoid wrap around from unsigned types
			// Accept channel_average as float incase mean etc. is used. Return as float
			float square_sum = 0.0f;
			for (std::size_t i_sample = 0; i_sample < get_number_of_spectra(); ++i_sample)
			{
				float d = static_cast<float>(get_sample(channel_index, i_sample)) - channel_average;
				square_sum += d * d;
			}

			return std::sqrt(square_sum / static_cast<float>(get_number_of_spectra()));
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_floating_point<T>::value, float>::type
		calculate_channel_standard_deviation(ChannelCount channel_index, DataType channel_average) const
		{
			DataType square_sum = 0.0;
			for (std::size_t i_sample = 0; i_sample < get_number_of_spectra(); ++i_sample)
			{
				DataType d = get_sample(channel_index, i_sample) - channel_average;
				square_sum += d * d;
			}

			return std::sqrt(square_sum / static_cast<float>(get_number_of_spectra()));
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_integral<T>::value, bool>::type
		is_equal(const TimeFrequency& test_data) const
		{
			if (this == &test_data)
				return true;

			if (!_metadata.is_equal(test_data._metadata))
				return false;

			for (std::size_t i = 0; i < get_total_samples(); ++i)
			{
				if (_data[i] != test_data._data[i])
					return false;
			}
			return true;
		}

		template<typename T = DataType>
		typename std::enable_if<std::is_floating_point<T>::value, bool>::type
		is_equal(const TimeFrequency& test_data) const
		{
			if (this == &test_data)
				return true;

			if (!_metadata.is_equal(test_data._metadata))
				return false;

			constexpr float tolerence = 1e-6f;
			for (std::size_t i = 0; i < get_total_samples(); ++i)
			{
				if (std::fabs(_data[i] - test_data._data[i]) > tolerence)
					return false;
			}

			return true;
		}

		DataType& get_sample(ChannelCount channel, SpectraCount sample)
		{
			assert(channel >= 0 && sample >= 0);
			assert(channel < _metadata._frequency_channels && sample < _metadata._number_of_spectra);
			return _data[channel * _metadata._number_of_spectra + sample];
		}
		const DataType& get_sample(ChannelCount channel, SpectraCount sample) const
		{
			assert(channel >= 0 && sample >= 0);
			assert(channel < _metadata._frequency_channels && sample < _metadata._number_of_spectra);
			return _data[channel * _metadata._number_of_spectra + sample];
		}

		DataType* get_raw() { return _data; }

		DataType* get_raw_channel_start(ChannelCount channel = 0) 
		{
			assert(channel >= 0);
			assert(channel < _metadata._frequency_channels);
			return &_data[channel * _metadata._number_of_spectra];
		}

		DataType* get_raw_channel_end(ChannelCount channel = 0)
		{
			assert(channel >= 0);
			assert(channel < _metadata._frequency_channels);
			return &_data[channel * _metadata._number_of_spectra + (_metadata._number_of_spectra - 1)];
		}

		ChannelCount get_number_of_channels() const { return _metadata._frequency_channels; }
		SpectraCount get_number_of_spectra() const { return _metadata._number_of_spectra; }
		std::size_t get_total_samples() const { return _metadata._frequency_channels * _metadata._number_of_spectra; }
		TimeFrequencyMetadata get_metadata() const { return _metadata; }

		TimeFrequency& operator=(const TimeFrequency&) = delete;

	private:
		const TimeFrequencyMetadata _metadata;
		DataType* _data;
	};
	
} // namespace rfim
#endif