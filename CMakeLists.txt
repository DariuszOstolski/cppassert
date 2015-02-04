cmake_minimum_required (VERSION 2.8)
project (cppassert)

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

set(CPPASSERT_LIBNAME ${CMAKE_PROJECT_NAME})
set(CPP_ASSERT_REQURED_LIBS)
set(CPP_ASSERT_REQURED_INCLUDE_DIRS)

if(Backtrace_FOUND)
    set(CMAKE_CXX_FLAGS "-DCPP_ASSERT_HAVE_BACKTRACE=1 ${CMAKE_CXX_FLAGS}")
    set(CPP_ASSERT_REQURED_LIBS ${Backtrace_LIBRARIES})
    set(CPP_ASSERT_REQURED_INCLUDE_DIRS ${Backtrace_INCLUDE_DIRS})
endif(Backtrace_FOUND)

IF (WIN32)
    set(CPP_ASSERT_REQURED_LIBS DbgHelp.lib)
ENDIF()

include_directories (include)
add_subdirectory (source)
add_subdirectory (samples)


enable_testing()
add_subdirectory (${PROJECT_SOURCE_DIR}/3rdparty/gtest-1.7.0)
add_subdirectory (tests)

