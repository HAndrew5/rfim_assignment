cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rfim_assignment)

add_subdirectory(rfim)

option (RFIM_ASSIGNMENT_INCLUDE_RFIM_TESTS "Include the suite of unit tests for rfim" ON)
option (RFIM_ASSIGNMENT_INCLUDE_RFIM_DEMO "Include the demonstration showing basic usage of the rfim library" ON)

if(RFIM_ASSIGNMENT_INCLUDE_RFIM_TESTS)
    add_subdirectory(rfim_tests)
endif()

if(RFIM_ASSIGNMENT_INCLUDE_RFIM_DEMO)
    add_subdirectory(rfim_demo)
endif()