#ifndef INCLUDE_RFIM_FILE_PROCESSOR
#define INCLUDE_RFIM_FILE_PROCESSOR

#include"TimeFrequency.h"
#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/DataWriter.h"

namespace rfim {

	template<typename StrategyType>
	class FileProcessor
	{
	public:

		using DataType = typename StrategyType::StrategyDataType;

		FileProcessor(StrategyType rfi_module, TimeFrequencyMetadata chunk_info) :
			_rfi_module(rfi_module),
			_chunk_info(chunk_info)
		{}

		std::size_t process_file(std::string source_filepath, std::string destination_filepath)
		{
			TimeFrequency<DataType>data_buffer(_chunk_info);
			DataReader reader(source_filepath);
			DataWriter writer(destination_filepath);
			std::size_t number_of_whole_chunks = reader.get_file_length<DataType>() / data_buffer.get_total_samples();

			for (std::size_t i = 0; i < number_of_whole_chunks; ++i)
			{
				reader.read_time_frequency_data_from_file(data_buffer);
				_rfi_module.process(data_buffer);
				writer.write_time_frequency_data_to_file(data_buffer);
			}

			return number_of_whole_chunks;
		}

	private:
		StrategyType _rfi_module;
		TimeFrequencyMetadata _chunk_info;
	};
} // namespace: rfim
#endif