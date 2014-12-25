# This is a toolchain file may  with information about
# arm compiler and paths.
#
# To use it just add -DCMAKE_TOOLCHAIN_FILE=../cmake/Modules/Arm6.cmake
#

set(CMAKE_SYSTEM_NAME Linux)


if(NOT DEFINED TOOLCHAIN_PREFIX)
    set(TOOLCHAIN_PREFIX "arm-linux-gnueabihf"
    CACHE
    STRING
    "Toolchain prefix for ARM6 32bit"
    FORCE)
    message("TOOLCHAIN_PREFIX not set. Using:  ${TOOLCHAIN_PREFIX}  used as default.")
endif(NOT DEFINED TOOLCHAIN_PREFIX)


SET(CMAKE_FIND_ROOT_PATH  "/usr/${TOOLCHAIN_PREFIX}")
SET(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}-gcc")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++")

