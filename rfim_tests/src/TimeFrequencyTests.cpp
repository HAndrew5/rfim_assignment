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