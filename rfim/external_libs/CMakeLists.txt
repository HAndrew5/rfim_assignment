#--------download and setup CPM if not already available-----
set (CPM_DOWNLOAD_VERSION 0.38.7)
set (CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
	message (STATUS "Downloading CPM.cmake")
	file (DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
		  ${CPM_DOWNLOAD_LOCATION}
	)
else (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
	message ("CPM already exists, do not need to download")
endif (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))

include (${CPM_DOWNLOAD_LOCATION})
#------------------------------

CPMAddPackage(
  NAME GroupSourcesByFolder.cmake
  GITHUB_REPOSITORY TheLartians/GroupSourcesByFolder.cmake
  VERSION 1.0
)

find_package(OpenMP REQUIRED)

target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)