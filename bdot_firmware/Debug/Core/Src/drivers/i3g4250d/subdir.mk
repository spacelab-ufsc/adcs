################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/i3g4250d/i3g4250d.c 

OBJS += \
./Core/Src/drivers/i3g4250d/i3g4250d.o 

C_DEPS += \
./Core/Src/drivers/i3g4250d/i3g4250d.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/i3g4250d/%.o Core/Src/drivers/i3g4250d/%.su Core/Src/drivers/i3g4250d/%.cyclo: ../Core/Src/drivers/i3g4250d/%.c Core/Src/drivers/i3g4250d/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers-2f-i3g4250d

clean-Core-2f-Src-2f-drivers-2f-i3g4250d:
	-$(RM) ./Core/Src/drivers/i3g4250d/i3g4250d.cyclo ./Core/Src/drivers/i3g4250d/i3g4250d.d ./Core/Src/drivers/i3g4250d/i3g4250d.o ./Core/Src/drivers/i3g4250d/i3g4250d.su

.PHONY: clean-Core-2f-Src-2f-drivers-2f-i3g4250d

