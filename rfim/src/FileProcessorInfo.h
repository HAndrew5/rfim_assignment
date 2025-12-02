#ifndef INCLUDE_RFIM_FILE_PROCESSOR_INFO
#define INCLUDE_RFIM_FILE_PROCESSOR_INFO


namespace rfim {

	struct FileProcessorInfo
	{
		FileProcessorInfo(size_t number_of_cleaned_channels=0, size_t number_of_procesed_chunks=0, 
			double processing_time=0.0) :
			_number_of_cleaned_channels(number_of_cleaned_channels),
			_number_of_procesed_chunks(number_of_procesed_chunks),
			_processing_milliseconds(processing_time)
		{
		}

		size_t _number_of_cleaned_channels;
		size_t _number_of_procesed_chunks;
		double _processing_milliseconds;
	};
} // namespace rfim
#endif