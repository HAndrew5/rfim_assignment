#include"DataReader.h"

namespace rfim {

	DataReader::DataReader(std::string file_path) :
		_in_stream(file_path, std::ios::binary)
	{
		if (!_in_stream.is_open())
		{
			throw std::runtime_error(
				std::string("Failed to open the file '") + file_path + "' when creating rfim::DataReader");
		}
	}

	DataReader::~DataReader()
	{
		_in_stream.close();
	}
} // namespace: rfim