#include"gtest/gtest.h"

#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MedianStandardDeviationRfi.h"
#include"../../rfim/src/FileProcessor.h"


TEST(FileProcessor, ProcessMedianRfiTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);
	std::string destination_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/median_cleaned_data.bin", __FILE__);

	rfim::TimeFrequencyMetadata metadata;
	rfim::MedianStandardDeviationRfi<float> rfi_module(metadata);
	rfim::FileProcessor<rfim::MedianStandardDeviationRfi<float>> processor(rfi_module, metadata);

	rfim::FileProcessorInfo info = processor.process_file(source_file_path, destination_file_path);
	EXPECT_EQ(info._number_of_procesed_chunks, 2);
	EXPECT_GT(info._number_of_cleaned_channels, 100);
}