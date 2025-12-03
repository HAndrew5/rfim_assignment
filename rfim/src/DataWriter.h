#ifndef INCLUDE_RFIM_DATA_WRITER
#define INCLUDE_RFIM_DATA_WRITER

#include <iostream>
#include <fstream>

#include"TimeFrequency.h"

namespace rfim {

	/*
	This class writes TimeFrequency data to a binary file.
	Data will be saved in a format with frequency channel major ordering i.e:
	[channel 0 sample 0], [channel 0 sample 1], ... [channel 0 sample N-1], [channel 1 sample 0]
	[channel 1 sample 1], ... [channel 1  sample N-1], ... [channel M-1 sample N-1]
	*/
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