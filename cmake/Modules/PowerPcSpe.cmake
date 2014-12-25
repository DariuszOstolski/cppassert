# This is a toolchain file may  with information about
# PowerPc with SPE ABI compiler and paths.
#
# To use it just add -DCMAKE_TOOLCHAIN_FILE=../cmake/Modules/PowerPcSpe.cmake
#

set(CMAKE_SYSTEM_NAME Linux)


set(TOOLCHAIN_PREFIX "powerpc-softfloat-linux-gnuspe"
    CACHE
    STRING
    "Toolchain prefix for PowerPC 32bit (SPE ABI)."
    FORCE)

SET(CMAKE_FIND_ROOT_PATH  "/usr/${TOOLCHAIN_PREFIX}")
SET(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}-gcc")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++")

