#ifndef INCLUDE_RFIM_TIME_FREQUENCY
#define INCLUDE_RFIM_TIME_FREQUENCY

#include<string>
#include<type_traits>
#include<cmath>
#include<cassert>
#include<stdexcept>

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
			_data(new DataType[_metadata._number_of_spectra * _metadata._frequency_channels])
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

		template<typename T = DataType>
		typename std::enable_if<std::is_integral<T>::value, bool>::type
		is_equal(const TimeFrequency& test_data) const
		{
			if (this == &test_data)
				return true;

			if (!_metadata.is_equal(test_data._metadata))
				return false;

			for (size_t i = 0; i < get_total_samples(); ++i)
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
			for (size_t i = 0; i < get_total_samples(); ++i)
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