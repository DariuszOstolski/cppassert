cmake_minimum_required(VERSION 3.16)

# Add project_options v0.16.0
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/cpp-best-practices/project_options/archive/refs/tags/v0.16.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

include("./Features.cmake")

# # Setup vcpkg - should be called before defining project()
run_vcpkg()

# Set the project name and language
set(PROJECT_VERSION "0.1.0")
project(cppassert VERSION ${PROJECT_VERSION} LANGUAGES CXX C)

if(${FEATURE_DOCS})
  set(ENABLE_DOXYGEN "ENABLE_DOXYGEN")
endif()

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment the options to enable them:
project_options(
      ENABLE_CACHE
      ENABLE_CPPCHECK
      # ENABLE_CLANG_TIDY # TODO
      # ENABLE_IPO
      ${ENABLE_DOXYGEN}
      # ENABLE_COVERAGE
      # WARNINGS_AS_ERRORS
      # ENABLE_SANITIZER_ADDRESS
      # ENABLE_SANITIZER_LEAK
      # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
      # ENABLE_SANITIZER_THREAD
      # ENABLE_SANITIZER_MEMORY
      # ENABLE_INCLUDE_WHAT_YOU_USE
      # ENABLE_PCH
      # PCH_HEADERS
      # ENABLE_USER_LINKER
      # ENABLE_BUILD_WITH_TIME_TRACE
      # ENABLE_UNITY
)

message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

message("module path: ${CMAKE_MODULE_PATH}")
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/FindBacktrace.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/Compilers.cmake)

include("./Source.cmake")

if(${FEATURE_SAMPLES})
  add_subdirectory (samples)
endif()

if(${FEATURE_TESTS})
  enable_testing()
  add_subdirectory (tests)
endif()
