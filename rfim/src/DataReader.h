#ifndef INCLUDE_RFIM_DATA_READER
#define INCLUDE_RFIM_DATA_READER

#include <iostream>
#include <fstream>

#include"TimeFrequency.h"

namespace rfim {

	/*
	This class reads TimeFrequency data from a binary file.
	Data is expected to be in a binary file with frequency channel major ordering i.e:
	[channel 0 sample 0], [channel 0 sample 1], ... [channel 0 sample N-1], [channel 1 sample 0]
	[channel 1 sample 1], ... [channel 1  sample N-1], ... [channel M-1 sample N-1]
	*/
	class DataReader
	{
	public:
		DataReader(std::string file_path);
		~DataReader();

		size_t get_file_length_bytes() { return _file_size; }
		size_t get_remaining_file_length_bytes();

		template <typename DataType>
		size_t get_file_length()
		{
			return get_file_length_bytes() / sizeof(DataType);
		}

		template <typename DataType>
		size_t get_remaining_file_length()
		{
			return get_remaining_file_length_bytes() / sizeof(DataType);
		}

		template <typename DataType>
		void read_time_frequency_data_from_file(TimeFrequency<DataType>& out_buffer)
		{
			_in_stream.read(reinterpret_cast<char*>(out_buffer.get_raw()), out_buffer.get_total_samples() * sizeof(DataType));

			if (_in_stream.fail() && _in_stream.eof())
				throw std::runtime_error(
					std::string("Failed to read as data past the end of the file was requested in rfim::DataReader.read_time_frequency_data_from_file"));

			if (_in_stream.bad() || _in_stream.fail())
				throw std::runtime_error(
					std::string("Failed to read from file in rfim::DataReader.read_time_frequency_data_from_file"));
		}

	private:
		std::ifstream _in_stream;
		size_t _file_size;
	};

} // namespace: rfim
#endif