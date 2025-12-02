#ifndef INCLUDE_RFIM_FILE_PROCESSOR
#define INCLUDE_RFIM_FILE_PROCESSOR

#include<chrono>

#include"FileProcessorInfo.h"
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

		FileProcessorInfo process_file(std::string source_filepath, std::string destination_filepath)
		{
			TimeFrequency<DataType>data_buffer(_chunk_info);
			DataReader reader(source_filepath);
			DataWriter writer(destination_filepath);
			std::size_t number_of_whole_chunks = reader.get_file_length<DataType>() / data_buffer.get_total_samples();
			std::size_t number_of_cleaned_channels = 0;
			double total_time = 0.0;

			for (std::size_t i = 0; i < number_of_whole_chunks; ++i)
			{
				reader.read_time_frequency_data_from_file(data_buffer);

				auto start_time = std::chrono::steady_clock::now();
				number_of_cleaned_channels += _rfi_module.process(data_buffer);
				auto end_time = std::chrono::steady_clock::now();
				
				std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
				total_time += elapsed.count();
				writer.write_time_frequency_data_to_file(data_buffer);
			}

			return FileProcessorInfo(number_of_cleaned_channels, number_of_whole_chunks, total_time);
		}

	private:
		StrategyType _rfi_module;
		TimeFrequencyMetadata _chunk_info;
	};
} // namespace: rfim
#endif