#include"DataWriter.h"

namespace rfim {

	DataWriter::DataWriter(std::string file_path) :
		_out_stream(file_path, std::ios::binary)
	{
		if (!_out_stream.is_open())
		{
			throw std::runtime_error(
				std::string("Failed to open the file '") + file_path + "' when creating rfim::DataWriter");
		}
	}

	DataWriter::~DataWriter()
	{
		_out_stream.close();
	}
} // namespace: rfim