CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG v1.14.0
  OPTIONS
  	"BUILD_GMOCK OFF"
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON"
    "BUILD_SHARED_LIBS OFF"
)

if (googletest_ADDED)
#googletest sets the output directories to the root of the build folder
#reset them here to put them in the CPM _deps/googletest-build/googletest folder
  set_target_properties (
    gtest gtest_main
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${googletest_BINARY_DIR}/googletest
      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${googletest_BINARY_DIR}/googletest/Debug
      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${googletest_BINARY_DIR}/googletest/Release
      RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${googletest_BINARY_DIR}/googletest/MinSizeRel
      RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${googletest_BINARY_DIR}/googletest/RelWithDebInfo
      ARCHIVE_OUTPUT_DIRECTORY ${googletest_BINARY_DIR}/googletest
      ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${googletest_BINARY_DIR}/googletest/Debug
      ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${googletest_BINARY_DIR}/googletest/Release
      ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${googletest_BINARY_DIR}/googletest/MinSizeRel
      ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${googletest_BINARY_DIR}/googletest/RelWithDebInfo
      LIBRARY_OUTPUT_DIRECTORY ${googletest_BINARY_DIR}/googletest
      LIBRARY_OUTPUT_DIRECTORY_DEBUG ${googletest_BINARY_DIR}/googletest/Debug
      LIBRARY_OUTPUT_DIRECTORY_RELEASE ${googletest_BINARY_DIR}/googletest/Release
      LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${googletest_BINARY_DIR}/googletest/MinSizeRel
      LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${googletest_BINARY_DIR}/googletest/RelWithDebInfo
      PDB_OUTPUT_DIRECTORY ${googletest_BINARY_DIR}/googletest
      PDB_OUTPUT_DIRECTORY_DEBUG ${googletest_BINARY_DIR}/googletest/Debug
      PDB_OUTPUT_DIRECTORY_RELEASE ${googletest_BINARY_DIR}/googletest/Release
      PDB_OUTPUT_DIRECTORY_MINSIZEREL ${googletest_BINARY_DIR}/googletest/MinSizeRel
      PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO ${googletest_BINARY_DIR}/googletest/RelWithDebInfo
      COMPILE_PDB_OUTPUT_DIRECTORY ${googletest_BINARY_DIR}/googletest
  )
    

	set_property (TARGET gtest PROPERTY FOLDER "external_libs")
	set_property (TARGET gtest_main PROPERTY FOLDER "external_libs")
endif()