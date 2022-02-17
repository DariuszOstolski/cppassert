set(srcs
    ./source/details/AssertionMessage.cpp
    ./source/details/DebugPrint.cpp
    ./source/details/Helpers.cpp
    ./source/details/StackTrace.cpp
    ./source/Assertion.cpp
    ./source/AssertionFailure.cpp
    ./source/CppAssert.cpp)

add_library(cppassert STATIC ${srcs})

# PRIVATE includes
set(INCLUDE_DIR "include")
target_include_directories(cppassert
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR})

# Public includes
target_include_directories(
  cppassert
  INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_DIR}>"
            "$<INSTALL_INTERFACE:./${CMAKE_INSTALL_INCLUDEDIR}>")

# Dependencies
if(FEATURE_BACKTRACE)
  find_package(Backtrace)
  if(Backtrace_FOUND)
    target_link_libraries(cppassert PRIVATE ${Backtrace_LIBRARIES})
    target_compile_definitions(cppassert
                               PRIVATE "-DCPP_ASSERT_HAVE_BACKTRACE=1")
    target_include_directories(cppassert PRIVATE ${Backtrace_INCLUDE_DIRS})
  endif()

  if(WIN32)
    target_link_libraries(cppassert PRIVATE "DbgHelp.lib")
  endif()
endif()
