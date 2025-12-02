project(rfim_demo)

add_executable(${PROJECT_NAME} "")
add_dependencies(${PROJECT_NAME} rfim)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PUBLIC rfim)