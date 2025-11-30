#ifndef INCLUDE_RFIM_DATA_WRITER
#define INCLUDE_RFIM_DATA_WRITER

#include <iostream>
#include <fstream>

#include"TimeFrequency.h"

namespace rfim {

	class DataWriter
	{
	public:
		DataWriter(std::string file_path);
		~DataWriter();

		template <typename DataType>
		void write_time_frequency_data_to_file(TimeFrequency<DataType>& buffer)
		{
			_out_stream.write(reinterpret_cast<char*>(buffer.get_raw()), buffer.get_total_samples() * sizeof(DataType));

			if (_out_stream.bad() || _out_stream.fail())
				throw std::runtime_error(
					std::string("Failed to read from file in rfim::DataWriter.read_time_frequency_data"));
		}

	private:
		std::ofstream _out_stream;
	};

} // namespace: rfim

#endif