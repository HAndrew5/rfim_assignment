#include"DataReader.h"

#include<cassert>

namespace rfim {

	DataReader::DataReader(std::string file_path) :
		_in_stream(file_path, std::ios::binary)
	{
		if (!_in_stream.is_open())
		{
			throw std::runtime_error(
				std::string("Failed to open the file '") + file_path + "' when creating rfim::DataReader");
		}

		_in_stream.seekg(0, std::ios::end);
		std::streamsize file_size = _in_stream.tellg();
		if (file_size < 0)
		{
			throw std::runtime_error(
				std::string("Failed to find the length of file '") + file_path + "' when creating rfim::DataReader");
		}
		_file_size = static_cast<std::size_t>(file_size);
		_in_stream.seekg(0, std::ios::beg);
	}

	DataReader::~DataReader()
	{
		_in_stream.close();
	}
	
	std::size_t DataReader::get_remaining_file_length_bytes()
	{
		std::streampos current_position = _in_stream.tellg();
		if (current_position < 0) {
			throw std::runtime_error(
				std::string("Failed to find the cursor position in rfim::DataReader.get_remaining_file_length_bytes"));
		}

		assert(current_position <= _file_size);

		return _file_size - static_cast<std::size_t>(current_position);
	}
	
} // namespace: rfim