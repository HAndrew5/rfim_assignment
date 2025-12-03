# rfim_assignment
This repository contains a C++11 library to aid in removing unwanted radio frequency interference from astronomical signals (an RFIM).

# Build Guide
This project is built using a cmake build system (available here: https://cmake.org/). Uses CPM package manager for cmake to automatically download all dependencies to the build/_deps folder (source code: https://github.com/cpm-cmake/CPM.cmake). 

Supports: Debug, MinSizeRel, RelWithDebugInfo and Release build configurations.

### cmake flags
- `RFIM_ASSIGNMENT_INCLUDE_RFIM_TESTS`: Include the rfim_tests project (default TRUE)
- `RFIM_ASSIGNMENT_INCLUDE_RFIM_DEMO`: Include the rfim_demo project (default TRUE)

### Tested Platforms
- Windows 11: Visual Studio 2022

Other platforms should work, but have not been tested.

### Visual Studio
- Clone or download the repository to a local folder
- Run cmake (either use the GUI or from the terminal inside a manually created build folder) to generate a Visual Studio solution in the build folder
- Open the solution in Visual Studio and build rfim
- Alternatively, to run rfim_tests or rfim_demo right click on the  project in the solution explorer and "Set as Startup Project" then start the "Local Windows Debugger"


**Important:** 
Tests and demo expect there to be a folder with file at `/data/data.bin`.

The file should be the test data supplied for the assignment. Without this, the tests and demo will fail.
The file can be found here: https://drive.google.com/file/d/1B4xa0XBwbW4yFdL3htuDT0Zzeapi1jVl/view


# rfim
A library for removing RFI from time-frequency signals.

`FileProcessor` can be used to load data from a file (in chunks), apply a selected RFI strategy and save to file.

`TimeFrequency` A buffer for time-frequency data of a set size. It is templated over float, uint8_t and uint16_t data. Contains various methods for setting/getting data, reading/writing data and calculating some quantities like the median.

**RFI Strategies**
- `MedianStandardDeviationRfi` sets channels containing samples a set number of standard deviations from the median to the median
- `MadRfi` calculates the median of the absolute deviation (MAD) and sets channels containing samples a given number of MAD's from the median to the median
- `RudimentaryRfi` a basic implementation minimally modified from the supplied rfi_clean.cpp. Only included as a referench benchmark

These strategies are implemented using the CRTP pattern for compile-time polymorphism. A `FileProcessor` can be initialised with any of these. They can also be used on float, uint8_t and uint16_t data (except RudimentaryRfi can only handle float due to being mostly unmodified from the original example). New RFIM can be added by deriving from the RfiStrategy class.

`DataReader` and `DataWriter` are used to read/write TimeFrequency data to .bin files.

**Use Example**
```cpp
#include"MadRfi.h"
#include"FileProcessor.h"

rfim::TimeFrequencyMetadata metadata; // initialised with standard values
rfim::MadRfi<float> rfi_module(metadata); // create MAD RFIM to operate on float's
rfim::FileProcessor<rfim::MadRfi<float>> processor(rfi_module, metadata); // create processor using MAD
processor.process_file(source_file_path, destination_file_path); // clean data and save to file

```
**Example 2**
```cpp
#include"MedianStandardDeviationRfi.h"

rfim::TimeFrequencyMetadata metadata; // initialised with standard values
float threshold = 6.3f;
rfim::MedianStandardDeviationRfi<float> rfi_module(metadata, threshold); // create RFIM with given threshold to operate on float's
rfim::TimeFrequency<float> data_buffer(metadata); // create an empty time-frequency buffer
data_buffer.read_data_from_raw(some_float*); // load buffer with data
rfi_module.process(data_buffer); // clean the contents of data_buffer in-place using MedianStandardDeviationRfi
```


# rfim_tests
Unit tests to verify correctness of rfim. Uses the googletest framework: https://github.com/google/googletest


# rfim_demo
A demonstration of what rfim can do.

Creates an executable with console input that allows a user to select an RFIM module. The data.bin file will then be processed, and the cleaned file will be saved in the `/data` folder. 

Several statistics will also be displayed, such as the number of chunks in the file, number of channels where RFI was removed, and time taken to process.