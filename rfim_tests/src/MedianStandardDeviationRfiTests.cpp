#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MedianStandardDeviationRfi.h"
#include"../../rfim/src/FileProcessor.h"


TEST(MedianRfiTest, BasicTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);
	std::string destination_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/generated_test_data.bin", __FILE__);

	rfim::TimeFrequencyMetadata metadata;
	rfim::MedianStandardDeviationRfi<float> rfi_module;
	rfim::FileProcessor<rfim::MedianStandardDeviationRfi<float>> processor(rfi_module, metadata);

	EXPECT_EQ(processor.process_file(source_file_path, destination_file_path), 2);
}