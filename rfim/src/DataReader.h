#ifndef INCLUDE_RFIM_DATA_READER
#define INCLUDE_RFIM_DATA_READER

#include <iostream>
#include <fstream>

#include"TimeFrequency.h"

namespace rfim {

	class DataReader
	{
	public:
		DataReader(std::string file_path);
		~DataReader();

		template <typename DataType>
		void read_time_frequency_data(TimeFrequency<DataType>& out_buffer)
		{
			_in_stream.read(reinterpret_cast<char*>(out_buffer.get_raw()), out_buffer.get_total_samples() * sizeof(DataType));

			if (_in_stream.fail() && _in_stream.eof())
				throw std::runtime_error(
					std::string("Failed to read as data past the end of the file was requested in rfim::DataReader.read_time_frequency_data"));

			if (_in_stream.bad() || _in_stream.fail())
				throw std::runtime_error(
					std::string("Failed to read from file in rfim::DataReader.read_time_frequency_data"));
		}

	private:
		std::ifstream _in_stream;
	};

} // namespace: rfim

#endif