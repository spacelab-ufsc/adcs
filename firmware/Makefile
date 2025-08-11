TARGET=adcs_bdot

BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

PARTNUMBER=stm32f103c8t6
OPENCM3_FAMILY=STM32F1

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++
AS=arm-none-eabi-as
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

FLASH=st-flash

INC=.
CFLAGS=--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -I. -mcpu=cortex-m3 -mthumb -msoft-float -Iconfig/ -Ios/include/ -Ios/portable/GCC/ARM_CM3/ -D$(OPENCM3_FAMILY)
LDFLAGS=-nostartfiles --specs=nano.specs -Wl,--gc-sections,-Map,$(BUILD_DIR_ABS)/$(TARGET).map,-T$(PARTNUMBER).ld -mcpu=cortex-m3 -mthumb

.PHONY: all
all: libopencm3_stm32f1.a libs_rule drivers_rule os_rule system_rule tasks_rule utils_rule crt.o main.o
	$(LD) $(LDFLAGS) $(BUILD_DIR_ABS)/*.o -o $(BUILD_DIR_ABS)/$(TARGET).elf -L$(BUILD_DIR_ABS)/ -lopencm3_stm32f1 -lm -lpredict
	$(OBJCOPY) -O binary $(BUILD_DIR_ABS)/$(TARGET).elf $(BUILD_DIR_ABS)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR_ABS)/$(TARGET).elf

.PHONY: flash
flash:
	$(FLASH) --reset write $(BUILD_DIR_ABS)/$(TARGET).bin 0x08000000

.PHONY: clean
clean:
	$(MAKE) clean -C hal
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) clean -C libs
	rm $(BUILD_DIR_ABS)/*.o $(BUILD_DIR_ABS)/*.a $(BUILD_DIR_ABS)/*.elf $(BUILD_DIR_ABS)/*.bin $(BUILD_DIR_ABS)/*.map
	@rm -rf $(BUILD_DIR_ABS)

.PHONY: build_dir
build_dir:
	@mkdir -p build

main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_DIR_ABS)/$@

crt.o: crt.s
	$(AS) -o $(BUILD_DIR_ABS)/$@ $<

libopencm3_stm32f1.a: hal/Makefile build_dir
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) TARGETS='stm32/f1' -C hal
	mv hal/lib/libopencm3_stm32f1.a $(BUILD_DIR_ABS)/

devices_rule: devices/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) -C devices

drivers_rule: drivers/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C drivers

libs_rule: libs/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) -C libs

os_rule: os/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) -C os

system_rule: system/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C system

tasks_rule: tasks/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C tasks

utils_rule: utils/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR_ABS) OPENCM3_FAMILY=$(OPENCM3_FAMILY) -C utils
