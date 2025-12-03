#ifndef INCLUDE_RFIM_TIME_FREQUENCY_METADATA
#define INCLUDE_RFIM_TIME_FREQUENCY_METADATA

#include<cstddef>


namespace rfim {

	typedef float FrequencyMhz;
	typedef float TimeIntervalSeconds;
	typedef size_t ChannelCount;
	typedef size_t SpectraCount;

	/*
	A POD class that holds information associated with a TimeFrequency class
	Does not do any sanity checking on values
	Can consider adding this if it becomes a problem e.g if a user might accidentally set the channels to 0,
	leading to TimeFrequencies to be initialised with length 0 
	*/
	class TimeFrequencyMetadata
	{
	public:
		static const ChannelCount DEFAULT_FREQUENCY_CHANNELS;
		static const SpectraCount DEFAULT_NUMBER_OF_SPECTRA;
		static const FrequencyMhz DEFAULT_FIRST_CHANNEL_FREQUENCY;
		static const FrequencyMhz DEFAULT_LAST_CHANNEL_FREQUENCY;
		static const FrequencyMhz DEFAULT_CHANNEL_WIDTH;
		static const TimeIntervalSeconds DEFAULT_SAMPLING_TIME;

		TimeFrequencyMetadata() :
			_frequency_channels(DEFAULT_FREQUENCY_CHANNELS),
			_number_of_spectra(DEFAULT_NUMBER_OF_SPECTRA),
			_first_channel_frequency(DEFAULT_FIRST_CHANNEL_FREQUENCY),
			_last_channel_frequency(DEFAULT_LAST_CHANNEL_FREQUENCY),
			_channel_width(DEFAULT_CHANNEL_WIDTH),
			_sampling_time(DEFAULT_SAMPLING_TIME)
		{
		}

		ChannelCount _frequency_channels;
		SpectraCount _number_of_spectra;
		FrequencyMhz _first_channel_frequency;
		FrequencyMhz _last_channel_frequency;
		FrequencyMhz _channel_width;
		TimeIntervalSeconds _sampling_time;
		

		bool is_equal(const TimeFrequencyMetadata& test_data) const
		{
			if (_frequency_channels != test_data._frequency_channels ||
				_number_of_spectra != test_data._number_of_spectra ||
				_first_channel_frequency != test_data._first_channel_frequency ||
				_last_channel_frequency != test_data._last_channel_frequency ||
				_channel_width != test_data._channel_width ||
				_sampling_time != test_data._sampling_time)
				return false;
			
			return true;
		}
	};
} // namespace: rfim
#endif