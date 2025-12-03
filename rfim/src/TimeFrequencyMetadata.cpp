#include "TimeFrequencyMetadata.h"


namespace rfim {
    const ChannelCount TimeFrequencyMetadata::DEFAULT_FREQUENCY_CHANNELS = static_cast<ChannelCount>(4096);
    const SpectraCount TimeFrequencyMetadata::DEFAULT_NUMBER_OF_SPECTRA = static_cast<SpectraCount>(1e4);
    const FrequencyMhz TimeFrequencyMetadata::DEFAULT_FIRST_CHANNEL_FREQUENCY = static_cast<FrequencyMhz>(1700);
    const FrequencyMhz TimeFrequencyMetadata::DEFAULT_LAST_CHANNEL_FREQUENCY = static_cast<FrequencyMhz>(2000);
    const FrequencyMhz TimeFrequencyMetadata::DEFAULT_CHANNEL_WIDTH = static_cast<FrequencyMhz>((2000.0 - 1700.0) / 4096.0);
    const TimeIntervalSeconds TimeFrequencyMetadata::DEFAULT_SAMPLING_TIME = static_cast<TimeIntervalSeconds>(50e-6);
} // namespace: rfim