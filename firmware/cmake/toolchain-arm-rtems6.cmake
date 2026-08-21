# Cross-compilation toolchain file for the RTEMS 6 / arm-rtems6-stm32f4 BSP.
#
# Usage:
#   cmake -S . -B out -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-rtems6.cmake
#
# The RTEMS install location is environment-driven so this works unmodified
# on any machine/CI runner that has the toolchain installed elsewhere:
#   export RTEMS_PREFIX=/opt/rtems/6
#   export RTEMS_PKG_CONFIG_PATH=/opt/rtems/6/lib/pkgconfig

if(DEFINED ENV{RTEMS_PREFIX})
    set(RTEMS_PREFIX "$ENV{RTEMS_PREFIX}" CACHE PATH "RTEMS toolchain install prefix")
else()
    set(RTEMS_PREFIX "/rtems/rtems/6" CACHE PATH "RTEMS toolchain install prefix")
endif()

if(DEFINED ENV{RTEMS_PKG_CONFIG_PATH})
    set(RTEMS_PKG_CONFIG_PATH "$ENV{RTEMS_PKG_CONFIG_PATH}" CACHE PATH "PKG_CONFIG_PATH for the RTEMS BSP")
else()
    set(RTEMS_PKG_CONFIG_PATH "/rtems/src/rtems/build/arm/stm32f4" CACHE PATH "PKG_CONFIG_PATH for the RTEMS BSP")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # don't try to run the cross-compiled binary on the host

set(CMAKE_C_COMPILER   "${RTEMS_PREFIX}/bin/arm-rtems6-gcc")
set(CMAKE_CXX_COMPILER "${RTEMS_PREFIX}/bin/arm-rtems6-g++")
set(CMAKE_ASM_COMPILER "${RTEMS_PREFIX}/bin/arm-rtems6-gcc")

set(RTEMS_OBJCOPY "${RTEMS_PREFIX}/bin/arm-rtems6-objcopy" CACHE FILEPATH "arm-rtems6-objcopy")
set(RTEMS_SIZE    "${RTEMS_PREFIX}/bin/arm-rtems6-size" CACHE FILEPATH "arm-rtems6-size")
