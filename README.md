# rfim_assignment
This repository contains a C++11 library to aid in removing unwanted radio frequency interference from astronomical signals (an RFIM).

# rfim
The main library. This is built using a cmake build system (available here: https://cmake.org/). Uses CPM package manager for cmake to automatically download all dependencies to the build/_deps folder (source code: https://github.com/cpm-cmake/CPM.cmake). 

Supports: Debug, MinSizeRel, RelWithDebugInfo and Release build configurations.

### cmake flags:
- `RFIM_ASSIGNMENT_INCLUDE_RFIM_TESTS`: Include the frim_tests project (default TRUE)

### Tested Platforms
- Windows 11: Visual Studio 2022

### Visual Studio
- Clone or download the repository to a local folder
- Run cmake (either use the GUI or from the terminal inside a manually created build folder) to generate a Visual Studio solution in the build folder
- Open the solution in Visual Studio and build rfim
- Alternatively, to run rfim_tests right click on the  project in the solution explorer and "Set as Startup Project" then start the "Local Windows Debugger"


# rfim_tests
Unit tests to verify correctness of rfim. Uses the googletest framework: https://github.com/google/googletest

**Important:** 
Tests expect there to be a folder with file at `rfim_assignment/data/data.bin`.

The file should be the test data supplied for the assignment. Without this the tests will fail.