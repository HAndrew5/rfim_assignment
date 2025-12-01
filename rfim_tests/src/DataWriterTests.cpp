#include"gtest/gtest.h"

#include"../../rfim/src/DataWriter.h"
#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"


TEST(DataWriterTest, ReadNoDirectoryTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../doesnt exist directory/", __FILE__);

	// check constructor throws when file isnt found
	EXPECT_THROW(rfim::DataWriter test_reader(source_file_path),
		std::runtime_error);
}

TEST(DataWriterTest, DefaultWriteTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);
	std::string out_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/generated_test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	rfim::DataWriter test_writer(out_file_path);

	rfim::TimeFrequencyMetadata metadata;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	test_writer.write_time_frequency_data_to_file(data_buffer);
	
	// read the data that was just written to verify it is the same as the
	// original buffer
	rfim::DataReader second_reader(out_file_path);
	rfim::TimeFrequency<float> test_buffer(metadata);
	second_reader.read_time_frequency_data_from_file(test_buffer);
	EXPECT_TRUE(test_buffer.is_equal(data_buffer));
}