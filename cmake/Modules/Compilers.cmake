#
# The purpose of this file is to set apropriate and required compiler
# flags.
#
macro(fix_default_compiler_settings_)

  if (MSVC)
    # For MSVC, CMake sets certain flags to defaults we want to override.
    # This replacement code is taken from sample in the CMake Wiki at
    # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.

    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

      if (NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)

        # When Google Test is built as a shared library, it should also use
        # shared runtime libraries.  Otherwise, it may end up with multiple
        # copies of runtime library data in different modules, resulting in
        # hard-to-find crashes. When it is built as a static library, it is
        # preferable to use CRT as static libraries, as we don't have to rely
        # on CRT DLLs being available. CMake always defaults to using shared
        # CRT libraries, so we override that default here.
        string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
      endif()

      # We prefer more strict warning checking for building Google Test.
      # Replaces /W3 with /W4 in defaults.
      string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    # using Clang

    if(CMAKE_BUILD_TYPE STREQUAL Test)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    endif (CMAKE_BUILD_TYPE STREQUAL Test)
    set(CMAKE_CXX_FLAGS "-pedantic -W -Wall -std=c++11 ${CMAKE_CXX_FLAGS}")

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # using gcc
    if(CMAKE_BUILD_TYPE STREQUAL "Test")
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -O0 -fsanitize=address")
    endif (CMAKE_BUILD_TYPE STREQUAL "Test")
    set(CMAKE_CXX_FLAGS "-pedantic -Wextra -Wmissing-include-dirs -W -Wall -std=c++11 ${CMAKE_CXX_FLAGS}")
    
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7 
        AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0 )
        set(CMAKE_CXX_FLAGS "-Werror ${CMAKE_CXX_FLAGS}")
        message("Detected supported compiler version added -Werror flag")
    endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
  # using Intel C++
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    fix_default_compiler_settings_()
    # using Visual Studio C++
    # Newlines inside flags variables break CMake's NMake generator.
    # TODO(vladl@google.com): Add -RTCs and -RTCu to debug builds.

    set(CMAKE_CXX_FLAGS "-GS -W4 -WX -wd4251 -wd4275 -nologo -J -Zi")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32 -D_WIN32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTRICT -DWIN32_LEAN_AND_MEAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -EHsc -D_HAS_EXCEPTIONS=1")

    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 17.0 
        AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 20.0 )
        set(CMAKE_CXX_FLAGS "-WX ${CMAKE_CXX_FLAGS}")
        message("Detected supported compiler version added -WX flag")
    endif()


    set(gtest_force_shared_crt ON)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL SunPro)
    # using gcc
    if(CMAKE_BUILD_TYPE STREQUAL "Test")
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -xdebugformat=dwarf")
    endif (CMAKE_BUILD_TYPE STREQUAL "Test")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

endif()
