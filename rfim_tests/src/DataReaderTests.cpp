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
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_file_length_bytes(), 327680000);
}

TEST(DataReaderTest, FileLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_file_length<float>(), 81920000);
	EXPECT_EQ(test_reader.get_file_length<uint8_t>(), 327680000);
	EXPECT_EQ(test_reader.get_file_length<uint16_t>(), 163840000);
}

TEST(DataReaderTest, FileRemainingLengthBytesTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), 327680000);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	std::size_t expected_bytes = 327680000 - (data_buffer.get_total_samples()*sizeof(float));
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), expected_bytes);

	expected_bytes = 327680000 - (2*data_buffer.get_total_samples() * sizeof(float));
	test_reader.read_time_frequency_data_from_file(data_buffer);
	EXPECT_EQ(test_reader.get_remaining_file_length_bytes(), expected_bytes);
}

TEST(DataReaderTest, FloatFileRemainingLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	std::size_t expected_float = 81920000;
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), expected_float);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<float> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_float -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), expected_float);

	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_float -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<float>(), expected_float);
}

TEST(DataReaderTest, Uint8FileRemainingLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	std::size_t expected_uint8 = 327680000;
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), expected_uint8);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<uint8_t> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_uint8 -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), expected_uint8);

	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_uint8 -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<uint8_t>(), expected_uint8);
}

TEST(DataReaderTest, Uint16FileRemainingLengthTest)
{
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);

	rfim::DataReader test_reader(source_file_path);
	std::size_t expected_uint16 = 163840000;
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), expected_uint16);

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 5;
	rfim::TimeFrequency<uint16_t> data_buffer(metadata);
	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_uint16 -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), expected_uint16);

	test_reader.read_time_frequency_data_from_file(data_buffer);
	expected_uint16 -= data_buffer.get_total_samples();
	EXPECT_EQ(test_reader.get_remaining_file_length<uint16_t>(), expected_uint16);
}