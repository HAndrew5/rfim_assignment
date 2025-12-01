#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/RudimentaryRfi.h"


TEST(RudimentaryRfiTest, BasicTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 10;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	rfim::TimeFrequency<float> original_buffer(data_buffer);

	rfim::RudimentaryRfi<float> rfi_module;
	EXPECT_NO_THROW(rfi_module.process(data_buffer));
}