#include"gtest/gtest.h"

#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MedianStandardDeviationRfi.h"
#include"../../rfim/src/FileProcessor.h"


TEST(FileProcessor, BasicTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);
	std::string destination_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/processed_test_data.bin", __FILE__);

	rfim::TimeFrequencyMetadata metadata;
	//metadata._frequency_channels = 5;
	rfim::MedianStandardDeviationRfi<float> rfi_module(metadata);
	rfim::FileProcessor<rfim::MedianStandardDeviationRfi<float>> processor(rfi_module, metadata);

	EXPECT_EQ(processor.process_file(source_file_path, destination_file_path), 2);
}