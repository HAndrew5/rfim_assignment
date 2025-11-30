#ifndef INCLUDE_RFIM_TIME_FREQUENCY_METADATA
#define INCLUDE_RFIM_TIME_FREQUENCY_METADATA

namespace rfim {

	typedef float FrequencyMhz;
	typedef float TimeIntervalSeconds;
	typedef std::size_t ChannelCount;
	typedef std::size_t SpectraCount;

	class TimeFrequencyMetadata
	{
	public:
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

		static constexpr ChannelCount DEFAULT_FREQUENCY_CHANNELS = static_cast<ChannelCount>(4096);
		static constexpr SpectraCount DEFAULT_NUMBER_OF_SPECTRA = static_cast<ChannelCount>(1e4);
		static constexpr FrequencyMhz DEFAULT_FIRST_CHANNEL_FREQUENCY = static_cast<FrequencyMhz>(1700);
		static constexpr FrequencyMhz DEFAULT_LAST_CHANNEL_FREQUENCY = static_cast<FrequencyMhz>(2000);
		static constexpr FrequencyMhz DEFAULT_CHANNEL_WIDTH = static_cast<FrequencyMhz>((2000.0-1700.0) / 4096.0);
		static constexpr TimeIntervalSeconds DEFAULT_SAMPLING_TIME = static_cast<TimeIntervalSeconds>(50e-6);
	};
} // namespace rfim
#endif