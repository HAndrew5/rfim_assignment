#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MedianStandardDeviationRfi.h"
#include"../../rfim/src/FileProcessor.h"


template <typename T>
class MedianRfiTest : public ::testing::Test
{
public:
	using TF = rfim::TimeFrequency<T>;
};

using MyTypes = ::testing::Types<float, uint8_t, uint16_t>;
TYPED_TEST_SUITE(MedianRfiTest, MyTypes);


TYPED_TEST(MedianRfiTest, ConstructorTest)
{
	rfim::TimeFrequencyMetadata metadata;
	EXPECT_NO_THROW(rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata));
}

TYPED_TEST(MedianRfiTest, GetThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata);
	TypeParam expected_threshold;
	if (std::is_floating_point<TypeParam>::value)
		expected_threshold = 4.5;
	else if (std::is_integral<TypeParam>::value)
		expected_threshold = 5;

	EXPECT_EQ(rfi_module.get_threshold(), expected_threshold);
}

TYPED_TEST(MedianRfiTest, ConstructThresholdThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	TypeParam threshold = 25;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata, threshold);
	
	EXPECT_EQ(rfi_module.get_threshold(), threshold);
}

TYPED_TEST(MedianRfiTest, SingleDoesChannelContainRfiTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 500;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata);
	
	// test trivial case of 0's
	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));

	// test one big spike is found
	time_frequency.get_sample(0, 234) = 200;
	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));
}

TYPED_TEST(MedianRfiTest, MultipleDoesChannelContainRfiTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata);

	// test a big spike is found on multiple channels in the same TimeFrequency
	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	time_frequency.get_sample(0, 234) = 200;
	time_frequency.get_sample(2, 234) = 46;

	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 1, 0));
	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 2, 0));
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 3, 0));
}

TYPED_TEST(MedianRfiTest, ProcessTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata);

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (std::size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 200;;
	time_frequency.get_sample(2, 234) = 46;

	// test 2 spikes found and channels reset
	EXPECT_EQ(rfi_module.process(time_frequency), 2);
	for (std::size_t i = 0; i < time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(*(time_frequency.get_raw() + i), 1);
}

TYPED_TEST(MedianRfiTest, HighThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata, 100);

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (std::size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 23;;
	time_frequency.get_sample(2, 234) = 46;
	rfim::TimeFrequency<TypeParam> time_frequency_original(time_frequency);

	// test no spikes found when the threshold is initialised high
	EXPECT_EQ(rfi_module.process(time_frequency), 0);
	EXPECT_TRUE(time_frequency.is_equal(time_frequency_original));
}

TYPED_TEST(MedianRfiTest, WrongSizeTimeFrequencyTest)
{
	rfim::TimeFrequencyMetadata metadata;
	rfim::MedianStandardDeviationRfi<TypeParam> rfi_module(metadata);
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (std::size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 200;;
	time_frequency.get_sample(2, 234) = 46;

	// test throw when size of TimeFrequency array differs
	EXPECT_THROW(rfi_module.process(time_frequency), std::out_of_range);
}