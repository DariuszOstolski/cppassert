# This is a toolchain file may  with information about
# PowerPc compiler and paths.
#
# To use it just add -DCMAKE_TOOLCHAIN_FILE=../cmake/Modules/PowerPc.cmake
#

set(CMAKE_SYSTEM_NAME Linux)


if(NOT DEFINED TOOLCHAIN_PREFIX)
    set(TOOLCHAIN_PREFIX "powerpc-linux-gnu"
    CACHE
    STRING
    "Toolchain prefix for PowerPC 32bit (standard ABI)."
    FORCE)
    message("TOOLCHAIN_PREFIX not set. Using:  ${TOOLCHAIN_PREFIX}  used as default.")
endif(NOT DEFINED TOOLCHAIN_PREFIX)

SET(CMAKE_FIND_ROOT_PATH  "/usr/${TOOLCHAIN_PREFIX}")
SET(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}-gcc")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++")

