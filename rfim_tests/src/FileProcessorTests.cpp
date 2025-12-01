#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/RudimentaryRfi.h"
#include"../../rfim/src/FileProcessor.h"


TEST(FileProcessor, BasicTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);
	std::string destination_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/processed_test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::RudimentaryRfi<float> rfi_module;
	rfim::FileProcessor<rfim::RudimentaryRfi<float>> processor(rfi_module, metadata);

	EXPECT_EQ(processor.process_file(source_file_path, destination_file_path), 2);
}