################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sun_pos/sun_pos.c 

OBJS += \
./Core/Src/sun_pos/sun_pos.o 

C_DEPS += \
./Core/Src/sun_pos/sun_pos.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sun_pos/%.o Core/Src/sun_pos/%.su Core/Src/sun_pos/%.cyclo: ../Core/Src/sun_pos/%.c Core/Src/sun_pos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sun_pos

clean-Core-2f-Src-2f-sun_pos:
	-$(RM) ./Core/Src/sun_pos/sun_pos.cyclo ./Core/Src/sun_pos/sun_pos.d ./Core/Src/sun_pos/sun_pos.o ./Core/Src/sun_pos/sun_pos.su

.PHONY: clean-Core-2f-Src-2f-sun_pos

