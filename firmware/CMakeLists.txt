cmake_minimum_required(VERSION 3.15)

# Compiler selection, CMAKE_SYSTEM_NAME, etc. come from a toolchain file, not
# from here — configure with:
#   cmake -S . -B out -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-rtems6.cmake
# See cmake/toolchain-arm-rtems6.cmake for how to point it at a different
# RTEMS install (RTEMS_PREFIX / RTEMS_PKG_CONFIG_PATH env vars).
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Missing -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-rtems6.cmake")
endif()

project(adcs C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# BSP package to link against
set(ENV{PKG_CONFIG_PATH} "${RTEMS_PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTEMS REQUIRED arm-rtems6-stm32f4)

include_directories(${RTEMS_INCLUDE_DIRS})
link_directories(${RTEMS_LIBRARY_DIRS})

# app
add_executable(adcs
# --- DRIVERS ---
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/i3g4250d/i3g4250d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/spi_commons.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/telemetry/adcs_telemetry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/telemetry/telemetry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/uart/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/utils/HK_ADCS_STATUS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pus/pus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pus/pus_crc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pus/pus_dispatch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pus/pus_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/interrupts/uart_isr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/queues/queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/tasks/bdot_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/tasks/housekeeping_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/tasks/sendreports_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rtems/tasks/watchdogtask.cpp
)

# --- OTIMIZACAO EXTREMA DE TAMANHO PARA O .ELF ---
# -Os: Reduz o código ao máximo
# -ffunction-sections -fdata-sections: Prepara o terreno para o lixeiro
# -fno-exceptions -fno-rtti: Arranca o peso morto de tratamento de erros do C++
# -flto: Link Time Optimization (comprime o binário cruzando os arquivos)
target_compile_options(adcs PRIVATE ${RTEMS_CFLAGS}
    -Os
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-rtti
    -flto
)

# -Wl,--gc-sections: Remove código declarado e não usado
# -Wl,--strip-all: DELETA TODO O LIXO DE DEBUG. É isso que faz o .elf despencar de Megabytes para Kilobytes.
target_link_options(adcs PRIVATE ${RTEMS_LDFLAGS}
    -Wl,--gc-sections
    -Wl,--strip-all
    -flto
)

target_link_libraries(adcs PRIVATE ${RTEMS_LIBRARIES})

# post build: produce .bin/.elf and print a memory report
add_custom_command(
    TARGET adcs POST_BUILD
    COMMAND ${RTEMS_OBJCOPY} -O binary $<TARGET_FILE:adcs> ${CMAKE_SOURCE_DIR}/out/adcs.bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:adcs> ${CMAKE_SOURCE_DIR}/out/adcs.elf
    COMMAND ${CMAKE_COMMAND} -E echo "=== TAMANHO REAL NA MEMORIA DO SATELITE ==="
    COMMAND ${RTEMS_SIZE} $<TARGET_FILE:adcs>
    COMMENT "Finalizando build e gerando relatorio de memoria..."
)
