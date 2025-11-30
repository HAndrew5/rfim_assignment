project(rfim_tests)

add_executable(${PROJECT_NAME} "")
add_dependencies(${PROJECT_NAME} rfim)

add_subdirectory(external_libs)
add_subdirectory(src)

GroupSourcesByFolder(rfim_tests)

target_link_libraries(${PROJECT_NAME} PRIVATE rfim gtest gtest_main)