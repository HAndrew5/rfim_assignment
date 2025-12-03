#include<string>
#include<stdio.h>
#include<iostream>

#include"../../rfim/src/FileProcessor.h"
#include"../../rfim/src/MedianStandardDeviationRfi.h"
#include"../../rfim/src/RudimentaryRfi.h"
#include"../../rfim/src/GetAbsoluteFilepathFromRelative.h"
#include"../../rfim/src/MadRfi.h"


int main(void)
{
	std::cout << "Welcome to rfim_demo!\n";
	std::cout << "This will demonstrate basic usage of the library 'rfim' that contains methods for removing radio frequency interference from time-frequency data\n\n";
	std::cout << std::string(100, '=') + "\n";
	std::cout << "Select a type of RFIM:\n";
	std::cout << "* 1: MedianStandardDeviationRfi (Set whole channel to median if sample exceeds some threshold above standard deviation) \n";
	std::cout << "* 2: MadRfi (Set whole channel to median if sample exceeds some threshold above median absolute distribution) \n";
	std::cout << "* 9: RudimentaryRfi (Basic implementation based on provided 'code 'rfi_clean.cpp' WARNING: WILL TAKE A LONG TIME) \n";
	std::cout << "* q: Quit \n";
	std::cout << std::string(100, '=') + "\n\n";

	int char_from_console = getchar();

	if (char_from_console == 'q')
		return 0;

	// The file at source_file_path is known to contain 2 standard size chunks of float data
	std::string source_file_path = GetAbsoluteFilepathFromRelative(
		"../../data/data.bin", __FILE__);
	rfim::TimeFrequencyMetadata metadata;
	rfim::FileProcessorInfo info;
	std::string destination_file_path;

	if (char_from_console == '1')
	{
		destination_file_path = GetAbsoluteFilepathFromRelative(
			"../../data/demo_median_cleaned_data.bin", __FILE__);

		rfim::MedianStandardDeviationRfi<float> rfi_module(metadata);
		rfim::FileProcessor<rfim::MedianStandardDeviationRfi<float>> processor(rfi_module, metadata);

		std::cout << "Processing data using MedianStandardDeviationRfi...\n";
		info = processor.process_file(source_file_path, destination_file_path);
	}
	else if (char_from_console == '2')
	{
		destination_file_path = GetAbsoluteFilepathFromRelative(
			"../../data/demo_mad_cleaned_data.bin", __FILE__);

		rfim::MadRfi<float> rfi_module(metadata, 10);
		rfim::FileProcessor<rfim::MadRfi<float>> processor(rfi_module, metadata);

		std::cout << "Processing data using MadRfi...\n";
		info = processor.process_file(source_file_path, destination_file_path);
	}
	else if (char_from_console == '9')
	{
		destination_file_path = GetAbsoluteFilepathFromRelative(
			"../../data/demo_rudimentary_cleaned_data.bin", __FILE__);

		rfim::RudimentaryRfi<float> rfi_module;
		rfim::FileProcessor<rfim::RudimentaryRfi<float>> processor(rfi_module, metadata);

		std::cout << "Processing data using RudimentaryRfi...\n";
		info = processor.process_file(source_file_path, destination_file_path);
	}
	else
	{
		std::cout << "Input not recognised\n";
		std::cout << "Exiting\n";
		return 0;
	}

	std::cout << "Complete!\n";
	std::cout << "Saved cleaned file as:\n";
	std::cout << destination_file_path << "\n\n";
	std::cout << "* Processing took " + std::to_string(info._processing_milliseconds) + " ms\n";
	std::cout << "* Cleaned " + std::to_string(info._number_of_procesed_chunks) + " chunks of data\n";
	std::cout << "* Each chunk has " + std::to_string(metadata._frequency_channels) + 
		" frequency channels and " + std::to_string(metadata._number_of_spectra) + " spectra\n";
	std::cout << "* Removed RFI on " + std::to_string(info._number_of_cleaned_channels) + " channels over all chunks\n\n";

	return 0;
}