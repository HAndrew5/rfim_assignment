#include<memory>

#include"gtest/gtest.h"

#include"../../rfim/src/TimeFrequency.h"


template <typename T>
class TimeFrequencyTest : public ::testing::Test 
{
public:
	using TF = rfim::TimeFrequency<T>;
};

using MyTypes = ::testing::Types<float, uint8_t, uint16_t>;
TYPED_TEST_SUITE(TimeFrequencyTest, MyTypes);


TYPED_TEST(TimeFrequencyTest, ConstructorTest) 
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF new_time_frequency(metadata);
	
	// check metadata in TimeFrequency matches the original
	EXPECT_TRUE(metadata.is_equal(new_time_frequency.get_metadata()));

	// check for default (0) initialisation
	TypeParam default_value = static_cast<TypeParam>(0);
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(*(new_time_frequency.get_raw() + i), default_value);
}

TYPED_TEST(TimeFrequencyTest, CopyConstructorTest) 
{
	using TF = typename TestFixture::TF;

	TypeParam set_value = static_cast<TypeParam>(100);
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 2048;
	TF new_time_frequency(metadata);
	new_time_frequency.get_sample(0, 0) = set_value;
	TF copy_time_frequency(new_time_frequency);

	// check metadata in TimeFrequency matches the original
	EXPECT_TRUE(metadata.is_equal(copy_time_frequency.get_metadata()));

	// check a deep copy of data has been made
	TypeParam new_set_value = static_cast<TypeParam>(200);
	copy_time_frequency.get_sample(0, 0) = new_set_value;
	EXPECT_NE(copy_time_frequency.get_sample(0, 0), new_time_frequency.get_sample(0, 0));
}

TYPED_TEST(TimeFrequencyTest, EqualityTest) 
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF a(metadata);
	TF b(metadata);
	EXPECT_TRUE(a.is_equal(a)); // test for trivial equality
	EXPECT_TRUE(a.is_equal(b)); // test for equality

	rfim::TimeFrequencyMetadata metadata1;
	metadata1._number_of_spectra = 100;
	TF c(metadata1);
	EXPECT_FALSE(a.is_equal(c)); // test metadata is different
	EXPECT_FALSE(b.is_equal(c));

	TypeParam new_set_value = static_cast<TypeParam>(200);
	b.get_sample(0, 0) = new_set_value;
	EXPECT_FALSE(a.is_equal(b)); // test sample data is different
}

TYPED_TEST(TimeFrequencyTest, ReadFromRawTest) 
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._number_of_spectra = 10;
	metadata._frequency_channels = 20;
	TF new_time_frequency(metadata);

	std::unique_ptr<TypeParam[]> rawBuffer(new TypeParam[new_time_frequency.get_total_samples()]);
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		rawBuffer[i] = static_cast<TypeParam>(i);

	// test exception thrown on null arg
	EXPECT_THROW(new_time_frequency.read_data_from_raw(nullptr);, std::invalid_argument);

	new_time_frequency.read_data_from_raw(rawBuffer.get());

	// test that all data was read into TimeFrequency
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(rawBuffer[i], *(new_time_frequency.get_raw() + i));

	// test a deep copy has been made
	TypeParam new_set_value = static_cast<TypeParam>(200);
	new_time_frequency.get_sample(0, 0) = new_set_value;
	EXPECT_NE(rawBuffer[0], new_time_frequency.get_sample(0, 0));
}

TYPED_TEST(TimeFrequencyTest, WriteToRawTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._number_of_spectra = 10;
	metadata._frequency_channels = 20;
	TF new_time_frequency(metadata);
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		*(new_time_frequency.get_raw() + i) = static_cast<TypeParam>(i);

	std::unique_ptr<TypeParam[]> rawBuffer(new TypeParam[new_time_frequency.get_total_samples()]);

	// test exception thrown on null arg
	EXPECT_THROW(new_time_frequency.write_data_to_raw(nullptr); , std::invalid_argument);

	new_time_frequency.write_data_to_raw(rawBuffer.get());

	// test that all data was written from TimeFrequency
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(rawBuffer[i], *(new_time_frequency.get_raw() + i));

	// test a deep copy has been made
	TypeParam new_set_value = static_cast<TypeParam>(200);
	new_time_frequency.get_sample(0, 0) = new_set_value;
	EXPECT_NE(rawBuffer[0], new_time_frequency.get_sample(0, 0));
}

TYPED_TEST(TimeFrequencyTest, WriteToTimeFrequencyTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._number_of_spectra = 10;
	metadata._frequency_channels = 20;
	TF new_time_frequency(metadata);
	for (int i = 0; i < new_time_frequency.get_total_samples(); ++i)
		*(new_time_frequency.get_raw() + i) = static_cast<TypeParam>(i);

	// test equality after write
	TF destination_buffer(metadata);
	EXPECT_FALSE(new_time_frequency.is_equal(destination_buffer));
	new_time_frequency.write_data_to_time_frequency(destination_buffer);
	EXPECT_TRUE(new_time_frequency.is_equal(destination_buffer));

	// test a deep copy has been made
	destination_buffer.get_sample(0,0) = 100;
	EXPECT_FALSE(new_time_frequency.is_equal(destination_buffer));

	// Test exception thrown on different length destination
	metadata._number_of_spectra = 11;
	TF different_spectra_buffer(metadata);
	EXPECT_THROW(new_time_frequency.write_data_to_time_frequency(different_spectra_buffer);, std::out_of_range);
}

TYPED_TEST(TimeFrequencyTest, SetChannelToValueTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._number_of_spectra = 10;
	metadata._frequency_channels = 20;
	TF time_frequency(metadata);
	TypeParam value = 5;
	TypeParam zero_value = 0;
	

	//test only channel 0 has been set
	time_frequency.set_channel_to_value(0, value);
	for (int i = 0; i < time_frequency.get_number_of_spectra(); ++i)
		EXPECT_EQ(time_frequency.get_sample(0,i), value);
	for (int i = time_frequency.get_number_of_spectra(); i < time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(time_frequency.get_raw()[i], zero_value);

	// test setting last channel
	TypeParam second_value = 10;
	time_frequency.set_channel_to_value(19, second_value);
	for (int i = 0; i < time_frequency.get_number_of_spectra(); ++i)
		EXPECT_EQ(time_frequency.get_sample(0, i), value);
	for (int i = time_frequency.get_number_of_spectra(); i < time_frequency.get_total_samples() - time_frequency.get_number_of_spectra(); ++i)
		EXPECT_EQ(time_frequency.get_raw()[i], zero_value);
	for (int i = 0; i < time_frequency.get_number_of_spectra(); ++i)
		EXPECT_EQ(*(time_frequency.get_raw_channel_start(19)+i), second_value);
}

TYPED_TEST(TimeFrequencyTest, GetSampleTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF new_time_frequency(metadata);

	// check get default value at 0,0
	TypeParam expected_value = static_cast<TypeParam>(0);
	EXPECT_EQ(new_time_frequency.get_sample(0,0), expected_value);

	// check setting and getting a new value to 0,0
	TypeParam new_value = static_cast<TypeParam>(10);
	new_time_frequency.get_sample(0, 0) = new_value;
	EXPECT_EQ(new_time_frequency.get_sample(0, 0), new_value);
	EXPECT_EQ(*new_time_frequency.get_raw(), new_value);

	// check setting and getting a new value to end of data
	new_time_frequency.get_sample(metadata._frequency_channels-1, metadata._number_of_spectra-1) = new_value;
	EXPECT_EQ(new_time_frequency.get_sample(metadata._frequency_channels-1, metadata._number_of_spectra-1), new_value);
	EXPECT_EQ(*(new_time_frequency.get_raw()+metadata._frequency_channels * metadata._number_of_spectra - 1), new_value);
}

TYPED_TEST(TimeFrequencyTest, GetConstSampleTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	const TF new_time_frequency(metadata);

	// check get default value at 0,0
	TypeParam expected_value = static_cast<TypeParam>(0);
	EXPECT_EQ(new_time_frequency.get_sample(0, 0), expected_value);
}

TYPED_TEST(TimeFrequencyTest, GetRawTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF new_time_frequency(metadata);

	// check get default value st start
	TypeParam expected_value = static_cast<TypeParam>(0);
	EXPECT_EQ(*new_time_frequency.get_raw(), expected_value);

	// check setting and getting a new value at start
	TypeParam new_value = static_cast<TypeParam>(10);
	*new_time_frequency.get_raw() = new_value;
	EXPECT_EQ(*new_time_frequency.get_raw(), new_value);

	// check setting and getting a new value to end of data
	std::size_t end_index = (metadata._frequency_channels * metadata._number_of_spectra) - 1;
	*(new_time_frequency.get_raw() + end_index) = new_value;
	EXPECT_EQ(*(new_time_frequency.get_raw() + end_index), new_value);
}

TYPED_TEST(TimeFrequencyTest, GetRawChannelStartTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF new_time_frequency(metadata);

	// check get default value st start
	TypeParam expected_value = static_cast<TypeParam>(0);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_start(0), expected_value);

	// check setting and getting a new value at channel 1
	TypeParam new_value = static_cast<TypeParam>(10);
	*new_time_frequency.get_raw_channel_start(1) = new_value;
	std::size_t channel_one_index = metadata._number_of_spectra;
	EXPECT_EQ(*(new_time_frequency.get_raw()+ channel_one_index), new_value);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_start(1), new_value);

	// check setting and getting a new value at last channel
	*new_time_frequency.get_raw_channel_start(metadata._frequency_channels-1) = new_value;
	std::size_t channel_end_index = (metadata._frequency_channels-1) * metadata._number_of_spectra;
	EXPECT_EQ(*(new_time_frequency.get_raw() + channel_end_index), new_value);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_start(metadata._frequency_channels-1), new_value);
}

TYPED_TEST(TimeFrequencyTest, GetRawChannelEndTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	TF new_time_frequency(metadata);

	// check get default value st end
	TypeParam expected_value = static_cast<TypeParam>(0);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_end(0), expected_value);

	// check setting and getting a new value for channel 1
	TypeParam new_value = static_cast<TypeParam>(10);
	*new_time_frequency.get_raw_channel_end(1) = new_value;
	std::size_t channel_one_end_index = metadata._number_of_spectra + metadata._number_of_spectra - 1;
	EXPECT_EQ(*(new_time_frequency.get_raw() + channel_one_end_index), new_value);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_end(1), new_value);

	// check setting and getting a new value at last channel
	*new_time_frequency.get_raw_channel_end(metadata._frequency_channels - 1) = new_value;
	std::size_t channel_end_index = (metadata._frequency_channels * metadata._number_of_spectra) - 1;
	EXPECT_EQ(*(new_time_frequency.get_raw() + channel_end_index), new_value);
	EXPECT_EQ(*new_time_frequency.get_raw_channel_end(metadata._frequency_channels - 1), new_value);
}

TYPED_TEST(TimeFrequencyTest, GetNumberOfChannelsTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	const TF time_frequency(metadata);
	// check number of channels equals default metadata
	EXPECT_EQ(time_frequency.get_number_of_channels(), metadata._frequency_channels);

	metadata._frequency_channels = 12;
	const TF new_time_frequency(metadata);
	// check number of channels equals metadata
	EXPECT_EQ(new_time_frequency.get_number_of_channels(), metadata._frequency_channels);
}

TYPED_TEST(TimeFrequencyTest, GetNumberOfSpectraTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	const TF time_frequency(metadata);
	// check number of spectra equals default metadata
	EXPECT_EQ(time_frequency.get_number_of_spectra(), metadata._number_of_spectra);

	metadata._number_of_spectra = 15;
	const TF new_time_frequency(metadata);
	// check number of spectra equals metadata
	EXPECT_EQ(new_time_frequency.get_number_of_spectra(), metadata._number_of_spectra);
}

TYPED_TEST(TimeFrequencyTest, GetTotalSamplesTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	const TF time_frequency(metadata);
	// check total samples equals default metadata
	EXPECT_EQ(time_frequency.get_total_samples(), metadata._number_of_spectra * metadata._frequency_channels);

	metadata._number_of_spectra = 15;
	const TF new_time_frequency(metadata);
	// check total samples equals metadata
	EXPECT_EQ(new_time_frequency.get_total_samples(), metadata._number_of_spectra * metadata._frequency_channels);

	metadata._frequency_channels = 10;
	const TF third_time_frequency(metadata);
	// check total samples equals metadata
	EXPECT_EQ(third_time_frequency.get_total_samples(), metadata._number_of_spectra * metadata._frequency_channels);
}

TYPED_TEST(TimeFrequencyTest, GetMetadataTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	const TF time_frequency(metadata);
	// check metadata equals default metadata
	EXPECT_TRUE(metadata.is_equal(time_frequency.get_metadata()));

	metadata._number_of_spectra = 15;
	const TF new_time_frequency(metadata);
	// check metadata is equal
	EXPECT_TRUE(metadata.is_equal(new_time_frequency.get_metadata()));
}

TYPED_TEST(TimeFrequencyTest, BasicDestructiveCalculateChannelMedianTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 1;
	TF time_frequency(metadata);

	// test trivial case
	TypeParam expected_median = 5;
	time_frequency.get_sample(0, 0) = expected_median;
	EXPECT_EQ(time_frequency.destructive_calculate_channel_median(0), expected_median);
}

TYPED_TEST(TimeFrequencyTest, BasicDestructiveCalculateChannelMedianCorrputionTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 3;
	metadata._number_of_spectra = 7;
	TF time_frequency(metadata);
	TypeParam test_values[21] = { 1, 2, 50, 2, 5, 6, 4,  1, 2, 50, 2, 5, 6, 4,  1, 2, 50, 2, 5, 6, 4};
	TypeParam channel_values[7] = { 1, 2, 50, 2, 5, 6, 4 };
	time_frequency.read_data_from_raw(test_values);

	// test only selected channel is corrupted
	TypeParam expected_median = 4;
	EXPECT_EQ(time_frequency.destructive_calculate_channel_median(1), expected_median);
	for (std::size_t i = 0; i < 7; ++i)
	{
		EXPECT_EQ(time_frequency.get_sample(0,i), channel_values[i]);
		EXPECT_EQ(time_frequency.get_sample(2, i), channel_values[i]);
	}
}

TYPED_TEST(TimeFrequencyTest, OddDataDestructiveCalculateChannelMedianTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 7;
	TF time_frequency(metadata);

	// test finding median of odd length data
	TypeParam test_values[7] = {1, 2, 50, 2, 5, 6, 4};
	time_frequency.read_data_from_raw(test_values);
	TypeParam expected_median = 4;
	EXPECT_EQ(time_frequency.destructive_calculate_channel_median(0), expected_median);
}

TYPED_TEST(TimeFrequencyTest, EvenDataDestructiveCalculateChannelMedianTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 8;
	TF time_frequency(metadata);

	// test finding median of odd length data
	TypeParam test_values[8] = { 1, 2, 50, 2, 5, 6, 4, 7 };
	time_frequency.read_data_from_raw(test_values);
	TypeParam expected_median = 5;
	EXPECT_EQ(time_frequency.destructive_calculate_channel_median(0), expected_median);
}

TYPED_TEST(TimeFrequencyTest, TrivialCalculateChannelStandardDeviationTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 7;
	TF time_frequency(metadata);

	// test finding median of odd length data
	TypeParam test_values[7] = { 11, 11, 11, 11, 11, 11, 11 };
	time_frequency.read_data_from_raw(test_values);
	TypeParam median = time_frequency.destructive_calculate_channel_median(0);

	// test trivial STD
	EXPECT_EQ(time_frequency.calculate_channel_standard_deviation(0, median), 0);
}

TYPED_TEST(TimeFrequencyTest, ExpectedCalculateChannelStandardDeviationTest)
{
	using TF = typename TestFixture::TF;

	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 5;
	TF time_frequency(metadata);
	

	// test finding median of odd length data
	TypeParam test_values[5] = { 1, 3, 5, 7, 9 };
	time_frequency.read_data_from_raw(test_values);
	TF median_buffer(time_frequency);
	TypeParam median = median_buffer.destructive_calculate_channel_median(0);
	EXPECT_EQ(median, 5);
	float expected_std = std::sqrt(8);

	// test known STD
	EXPECT_EQ(time_frequency.calculate_channel_standard_deviation(0, median), expected_std);
}

TEST(TimeFrequencyUint8Test, CalculateChannelStandardDeviationOverflowTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 5;
	rfim::TimeFrequency<uint8_t> time_frequency(metadata);

	// test whether correct value is returned for values near to uint8 max
	uint8_t test_values[5] = { 100, 30, 50, 70, 200 };
	time_frequency.read_data_from_raw(test_values);
	rfim::TimeFrequency<uint8_t> median_buffer(time_frequency);
	uint8_t median = median_buffer.destructive_calculate_channel_median(0);
	EXPECT_EQ(median, 70);
	float expected_std = std::sqrt(3960);

	// test known STD
	EXPECT_EQ(time_frequency.calculate_channel_standard_deviation(0, median), expected_std);
}

TEST(TimeFrequencyUint16Test, CalculateChannelStandardDeviationOverflowTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 5;
	rfim::TimeFrequency<uint16_t> time_frequency(metadata);

	// test whether correct value is returned for values near to uint16 max
	uint16_t test_values[5] = { 65500, 50000, 10000, 40000, 5000 };
	time_frequency.read_data_from_raw(test_values);
	rfim::TimeFrequency<uint16_t> median_buffer(time_frequency);
	uint16_t median = median_buffer.destructive_calculate_channel_median(0);
	EXPECT_EQ(median, 40000);
	float expected_std = std::sqrt(575050000);

	// test known STD
	EXPECT_EQ(time_frequency.calculate_channel_standard_deviation(0, median), expected_std);
}