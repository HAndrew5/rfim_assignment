# rfim_assignment
This repository contains a C++11 library to aid in removing unwanted radio frequency interference from astronomical signals (an RFIM).

# rfim
The main library. This is built using a cmake build system (available here: https://cmake.org/). Uses CPM package manager for cmake to automatically download all dependencies to the build/_deps folder (source code: https://github.com/cpm-cmake/CPM.cmake). 

Supports: Debug, MinSizeRel, RelWithDebugInfo and Release build configurations.

### cmake flags
- `RFIM_ASSIGNMENT_INCLUDE_RFIM_TESTS`: Include the rfim_tests project (default TRUE)
- `RFIM_ASSIGNMENT_INCLUDE_RFIM_DEMO`: Include the rfim_demo project (default TRUE)

### Tested Platforms
- Windows 11: Visual Studio 2022

### Visual Studio
- Clone or download the repository to a local folder
- Run cmake (either use the GUI or from the terminal inside a manually created build folder) to generate a Visual Studio solution in the build folder
- Open the solution in Visual Studio and build rfim
- Alternatively, to run rfim_tests or rfim_demo right click on the  project in the solution explorer and "Set as Startup Project" then start the "Local Windows Debugger"


**Important:** 
Tests and demo expect there to be a folder with file at `/data/data.bin`.

The file should be the test data supplied for the assignment. Without this, the tests and demo will fail.
The file can be found here: https://drive.google.com/file/d/1B4xa0XBwbW4yFdL3htuDT0Zzeapi1jVl/view


# rfim_tests
Unit tests to verify correctness of rfim. Uses the googletest framework: https://github.com/google/googletest


# rfim_demo
A demonstration of what rfim can do.

Creates an executable with console input that allows a user to select an RFIM module. The data.bin file will then be processed, and the cleaned file will be saved in the `/data` folder. 

Several statistics will also be displayed, such as the number of chunks in the file, number of channels where RFI was removed, and time taken to process.