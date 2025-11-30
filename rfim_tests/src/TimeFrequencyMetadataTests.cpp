#include"gtest/gtest.h"

#include"../../rfim/src/TimeFrequencyMetadata.h"

TEST(SimpleTimeFrequencyTest, DefaultDefaultValuesTest)
{
	rfim::ChannelCount defaultFrequencyChannels = static_cast<rfim::ChannelCount>(4096);
	rfim::SpectraCount defaultNumberOfSpectra = static_cast<rfim::SpectraCount>(1e4);
	rfim::FrequencyMhz defaultStartFrequency = static_cast<rfim::FrequencyMhz>(1700);
	rfim::FrequencyMhz defaultEndFrequency = static_cast<rfim::FrequencyMhz>(2000);
	rfim::FrequencyMhz defaultChannelWidth = static_cast<rfim::FrequencyMhz>((defaultEndFrequency - defaultStartFrequency)
		/ static_cast<double>(defaultFrequencyChannels));
	rfim::TimeIntervalSeconds defaultSamplingTime = static_cast<rfim::TimeIntervalSeconds>(50e-6);

	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_FREQUENCY_CHANNELS, defaultFrequencyChannels);
	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_NUMBER_OF_SPECTRA, defaultNumberOfSpectra);
	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_FIRST_CHANNEL_FREQUENCY, defaultStartFrequency);
	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_LAST_CHANNEL_FREQUENCY, defaultEndFrequency);
	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_CHANNEL_WIDTH, defaultChannelWidth);
	EXPECT_EQ(rfim::TimeFrequencyMetadata::DEFAULT_SAMPLING_TIME, defaultSamplingTime);
}

TEST(SimpleTimeFrequencyTest, DefaultConstructionTest)
{
	rfim::TimeFrequencyMetadata metadata;
	EXPECT_EQ(metadata._frequency_channels, rfim::TimeFrequencyMetadata::DEFAULT_FREQUENCY_CHANNELS);
	EXPECT_EQ(metadata._number_of_spectra, rfim::TimeFrequencyMetadata::DEFAULT_NUMBER_OF_SPECTRA);
	EXPECT_EQ(metadata._first_channel_frequency, rfim::TimeFrequencyMetadata::DEFAULT_FIRST_CHANNEL_FREQUENCY);
	EXPECT_EQ(metadata._last_channel_frequency, rfim::TimeFrequencyMetadata::DEFAULT_LAST_CHANNEL_FREQUENCY);
	EXPECT_EQ(metadata._channel_width, rfim::TimeFrequencyMetadata::DEFAULT_CHANNEL_WIDTH);
	EXPECT_EQ(metadata._sampling_time, rfim::TimeFrequencyMetadata::DEFAULT_SAMPLING_TIME);
}

TEST(SimpleTimeFrequencyTest, ValueSetTest)
{
	rfim::TimeFrequencyMetadata metadata;

	rfim::ChannelCount newFrequencyChannels = static_cast<rfim::ChannelCount>(2048);
	metadata._frequency_channels = newFrequencyChannels;
	EXPECT_EQ(metadata._frequency_channels, newFrequencyChannels);

	rfim::SpectraCount newNumberOfSpectra = static_cast<rfim::SpectraCount>(2e4);
	metadata._number_of_spectra = newNumberOfSpectra;
	EXPECT_EQ(metadata._number_of_spectra, newNumberOfSpectra);

	rfim::FrequencyMhz newStartFrequency = static_cast<rfim::FrequencyMhz>(1500);
	metadata._first_channel_frequency = newStartFrequency;
	EXPECT_EQ(metadata._first_channel_frequency, newStartFrequency);

	rfim::FrequencyMhz newEndFrequency = static_cast<rfim::FrequencyMhz>(3500);
	metadata._last_channel_frequency = newEndFrequency;
	EXPECT_EQ(metadata._last_channel_frequency, newEndFrequency);

	rfim::FrequencyMhz newChannelWidth = static_cast<rfim::FrequencyMhz>(125.568);
	metadata._channel_width = newChannelWidth;
	EXPECT_EQ(metadata._channel_width, newChannelWidth);

	rfim::TimeIntervalSeconds newSamplingTime = static_cast<rfim::TimeIntervalSeconds>(34.6);
	metadata._channel_width = newSamplingTime;
	EXPECT_EQ(metadata._channel_width, newSamplingTime);
}

TEST(SimpleTimeFrequencyTest, IsEqualTest)
{
	rfim::TimeFrequencyMetadata metadata;
	EXPECT_TRUE(metadata.is_equal(metadata));
	
	rfim::TimeFrequencyMetadata frequency_channel_metadata;
	frequency_channel_metadata._frequency_channels = frequency_channel_metadata._frequency_channels + 1;
	EXPECT_FALSE(metadata.is_equal(frequency_channel_metadata));

	rfim::TimeFrequencyMetadata spectra_metadata;
	spectra_metadata._number_of_spectra = spectra_metadata._number_of_spectra + 1;
	EXPECT_FALSE(metadata.is_equal(spectra_metadata));

	rfim::TimeFrequencyMetadata first_channel_metadata;
	first_channel_metadata._first_channel_frequency = first_channel_metadata._first_channel_frequency + 1;
	EXPECT_FALSE(metadata.is_equal(first_channel_metadata));

	rfim::TimeFrequencyMetadata last_channel_metadata;
	last_channel_metadata._last_channel_frequency = last_channel_metadata._last_channel_frequency + 1;
	EXPECT_FALSE(metadata.is_equal(last_channel_metadata));

	rfim::TimeFrequencyMetadata width_metadata;
	width_metadata._channel_width = width_metadata._channel_width + 1;
	EXPECT_FALSE(metadata.is_equal(width_metadata));

	rfim::TimeFrequencyMetadata sample_time_metadata;
	sample_time_metadata._sampling_time = sample_time_metadata._sampling_time + 1;
	EXPECT_FALSE(metadata.is_equal(sample_time_metadata));
}