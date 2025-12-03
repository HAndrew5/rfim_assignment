#include"gtest/gtest.h"

#include"../../rfim/src/DataReader.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MadRfi.h"
#include"../../rfim/src/FileProcessor.h"


template <typename T>
class MadRfiTest : public ::testing::Test
{
public:
	using TF = rfim::TimeFrequency<T>;
};

using MyTypes = ::testing::Types<float, uint8_t, uint16_t>;
TYPED_TEST_SUITE(MadRfiTest, MyTypes);


TYPED_TEST(MadRfiTest, ConstructorTest)
{
	rfim::TimeFrequencyMetadata metadata;
	EXPECT_NO_THROW(rfim::MadRfi<TypeParam> rfi_module(metadata));
}

TYPED_TEST(MadRfiTest, GetThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	rfim::MadRfi<TypeParam> rfi_module(metadata);
	EXPECT_EQ(rfi_module.get_threshold(), 4.5f);
}

TYPED_TEST(MadRfiTest, ConstructThresholdThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	float threshold = 25.3;
	rfim::MadRfi<TypeParam> rfi_module(metadata, threshold);

	EXPECT_EQ(rfi_module.get_threshold(), threshold);
}

TYPED_TEST(MadRfiTest, SingleDoesChannelContainRfiTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 1;
	metadata._number_of_spectra = 500;
	rfim::MadRfi<TypeParam> rfi_module(metadata);

	// test trivial case of 0's
	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));

	// test one big spike is found
	time_frequency.get_sample(0, 234) = 200;
	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));
}

TYPED_TEST(MadRfiTest, MultipleDoesChannelContainRfiTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	rfim::MadRfi<TypeParam> rfi_module(metadata);

	// test a big spike is found on multiple channels in the same TimeFrequency
	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	time_frequency.get_sample(0, 234) = 200;
	time_frequency.get_sample(2, 234) = 46;

	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 0, 0));
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 1, 0));
	EXPECT_TRUE(rfi_module.does_channel_contain_rfi(time_frequency, 2, 0));
	EXPECT_FALSE(rfi_module.does_channel_contain_rfi(time_frequency, 3, 0));
}

TYPED_TEST(MadRfiTest, ProcessTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	rfim::MadRfi<TypeParam> rfi_module(metadata);

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 200;;
	time_frequency.get_sample(2, 234) = 46;

	// test 2 spikes found and channels reset
	EXPECT_EQ(rfi_module.process(time_frequency), 2);
	for (size_t i = 0; i < time_frequency.get_total_samples(); ++i)
		EXPECT_EQ(*(time_frequency.get_raw() + i), 1);
}

TYPED_TEST(MadRfiTest, HighThresholdTest)
{
	rfim::TimeFrequencyMetadata metadata;
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;
	float threshold;
	if (std::is_integral<TypeParam>::value)
		threshold = 100.0f;
	else
		threshold = 1e8f;
	
	rfim::MadRfi<TypeParam> rfi_module(metadata, threshold);

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 23;;
	time_frequency.get_sample(2, 234) = 46;
	rfim::TimeFrequency<TypeParam> time_frequency_original(time_frequency);

	// test no spikes found when the threshold is initialised very high
	EXPECT_EQ(rfi_module.process(time_frequency), 0);
	EXPECT_TRUE(time_frequency.is_equal(time_frequency_original));
}

TYPED_TEST(MadRfiTest, WrongSizeTimeFrequencyTest)
{
	rfim::TimeFrequencyMetadata metadata;
	rfim::MadRfi<TypeParam> rfi_module(metadata);
	metadata._frequency_channels = 4;
	metadata._number_of_spectra = 500;

	rfim::TimeFrequency<TypeParam> time_frequency(metadata);
	for (size_t i = 0; i < metadata._frequency_channels; ++i)
		time_frequency.set_channel_to_value(i, 1);
	time_frequency.get_sample(0, 234) = 200;;
	time_frequency.get_sample(2, 234) = 46;

	// test throw when size of TimeFrequency array differs
	EXPECT_THROW(rfi_module.process(time_frequency), std::out_of_range);
}