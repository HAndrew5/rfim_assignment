project(rfim)

add_library(${PROJECT_NAME} STATIC "")

add_subdirectory(external_libs)
add_subdirectory(src)

GroupSourcesByFolder(rfim)