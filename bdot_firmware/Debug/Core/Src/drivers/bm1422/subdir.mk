################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/bm1422/bm1422.c 

OBJS += \
./Core/Src/drivers/bm1422/bm1422.o 

C_DEPS += \
./Core/Src/drivers/bm1422/bm1422.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/bm1422/%.o Core/Src/drivers/bm1422/%.su Core/Src/drivers/bm1422/%.cyclo: ../Core/Src/drivers/bm1422/%.c Core/Src/drivers/bm1422/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers-2f-bm1422

clean-Core-2f-Src-2f-drivers-2f-bm1422:
	-$(RM) ./Core/Src/drivers/bm1422/bm1422.cyclo ./Core/Src/drivers/bm1422/bm1422.d ./Core/Src/drivers/bm1422/bm1422.o ./Core/Src/drivers/bm1422/bm1422.su

.PHONY: clean-Core-2f-Src-2f-drivers-2f-bm1422

