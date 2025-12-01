#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"


TEST(DataReaderTest, ReadNoFileTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/doesnt_exist.bin", __FILE__);

	// check constructor throws when file isnt found
	EXPECT_THROW(rfim::DataReader test_reader(source_file_path), 
		std::runtime_error);
}

TEST(DataReaderTest, DefaultReadTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);

	rfim::TimeFrequencyMetadata metadata;
	rfim::TimeFrequency<float> data_buffer(metadata);
	rfim::TimeFrequency<float> original_buffer(data_buffer);
	
	// show data has been read into TimeFrequency buffer
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_FALSE(data_buffer.is_equal(original_buffer));
}

TEST(DataReaderTest, DoubleReadTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);

	rfim::TimeFrequencyMetadata metadata;
	rfim::TimeFrequency<float> data_buffer(metadata);
	rfim::TimeFrequency<float> original_buffer(data_buffer);

	// show data has been read into TimeFrequency buffer
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_FALSE(data_buffer.is_equal(original_buffer));

	// show the second read is different to the first
	rfim::TimeFrequency<float> first_read_buffer(data_buffer);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_FALSE(data_buffer.is_equal(original_buffer));
	EXPECT_FALSE(data_buffer.is_equal(first_read_buffer));
}

TEST(DataReaderTest, FileEndTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);

	rfim::TimeFrequencyMetadata metadata;
	rfim::TimeFrequency<float> data_buffer(metadata);
	rfim::TimeFrequency<float> original_buffer(data_buffer);

	// show data has been read into TimeFrequency buffer
	test_reader.read_time_frequency_data_from_file(data_buffer);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_THROW(test_reader.read_time_frequency_data_from_file(data_buffer),
		std::runtime_error);
}

TEST(DataReaderTest, FileLengthBytesTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_file_length_bytes(), 400000);
}

TEST(DataReaderTest, FileLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_file_length<float>(), 100000);
	EXPECT_EQ(test_reader.get_file_length<uint8_t>(), 400000);
	EXPECT_EQ(test_reader.get_file_length<uint16_t>(), 200000);
}

TEST(DataReaderTest, FileRemainingLengthBytesTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), 400000);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), 200000);

	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), 0);
}

TEST(DataReaderTest, FileRemainingLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/test_data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), 100000);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), 400000);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), 200000);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), 50000);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), 200000);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), 100000);

	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), 0);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), 0);
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), 0);
}