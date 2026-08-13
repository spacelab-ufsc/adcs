cmake_minimum_required(VERSION 3.15)

# 1. FORÇAR COMPILADORES E DESATIVAR TESTES DE BINÁRIO
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # Não tenta rodar o executável no PC

set(CMAKE_C_COMPILER "/rtems/rtems/6/bin/arm-rtems6-gcc")
set(CMAKE_CXX_COMPILER "/rtems/rtems/6/bin/arm-rtems6-g++")
set(CMAKE_ASM_COMPILER "/rtems/rtems/6/bin/arm-rtems6-gcc")

project(adcs_bdot C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# config
set(ENV{PKG_CONFIG_PATH} "/rtems/src/rtems/build/arm/stm32f4")
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTEMS REQUIRED arm-rtems6-stm32f4)

# incluir lib rtems
include_directories(${RTEMS_INCLUDE_DIRS})
link_directories(${RTEMS_LIBRARY_DIRS})

# app
add_executable(app
# --- DRIVERS ---
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/i3g4250d/i3g4250d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/spi_commons.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/telemetry/adcs_telemetry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/telemetry/telemetry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/uart/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/utils/HK_ADCS_STATUS.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pus/pus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pus/pus_crc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pus/pus_dispatch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pus/pus_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/interrupts/uart_isr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/queues/queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/tasks/bdot_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/tasks/housekeeping_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/tasks/sendreports_task.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rtems/tasks/watchdogtask.cpp
)

# --- OTIMIZACAO EXTREMA DE TAMANHO PARA O .ELF ---
# -Os: Reduz o código ao máximo
# -ffunction-sections -fdata-sections: Prepara o terreno para o lixeiro
# -fno-exceptions -fno-rtti: Arranca o peso morto de tratamento de erros do C++
# -flto: Link Time Optimization (comprime o binário cruzando os arquivos)
target_compile_options(app PRIVATE ${RTEMS_CFLAGS} 
    -Os 
    -ffunction-sections 
    -fdata-sections 
    -fno-exceptions 
    -fno-rtti 
    -flto
)

# -Wl,--gc-sections: Remove código declarado e não usado
# -Wl,--strip-all: DELETA TODO O LIXO DE DEBUG. É isso que faz o .elf despencar de Megabytes para Kilobytes.
target_link_options(app PRIVATE ${RTEMS_LDFLAGS} 
    -Wl,--gc-sections 
    -Wl,--strip-all 
    -flto
)

target_link_libraries(app PRIVATE ${RTEMS_LIBRARIES})

# post build e relatorio
add_custom_command(
    TARGET app POST_BUILD
    COMMAND /rtems/rtems/6/bin/arm-rtems6-objcopy -O binary $<TARGET_FILE:app> ${CMAKE_SOURCE_DIR}/out/app.bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:app> ${CMAKE_SOURCE_DIR}/out/app.elf 
    COMMAND ${CMAKE_COMMAND} -E echo "=== OTIMIZACAO EXTREMA APLICADA ==="
    COMMAND /rtems/rtems/6/bin/arm-rtems6-size $<TARGET_FILE:app>
)

# post build e relatorio de memoria
add_custom_command(
    TARGET app POST_BUILD
    
    # 1. Gera o .bin
    COMMAND /rtems/rtems/6/bin/arm-rtems6-objcopy
    -O binary $<TARGET_FILE:app> ${CMAKE_SOURCE_DIR}/out/app.bin

    # 2. Copia o .elf original
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:app>
    ${CMAKE_SOURCE_DIR}/out/app.elf 
    
    # 3. Imprime o relatorio de tamanho real no terminal
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=== TAMANHO REAL NA MEMORIA DO SATELITE ==="
    COMMAND /rtems/rtems/6/bin/arm-rtems6-size $<TARGET_FILE:app>
    COMMAND ${CMAKE_COMMAND} -E echo "==========================================="
    
    COMMENT "Finalizando build e gerando relatorio de memoria..."
)