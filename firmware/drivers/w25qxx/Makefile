TARGET=w25qxxx

BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

PARTNUMBER=stm32f103c8t6

CC=arm-none-eabi-gcc
CPP=arm-none-eabi-g++
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

CFLAGS=-c -mcpu=cortex-m3 -mthumb -O0

.PHONY: all
all:
	$(CC) $(CFLAGS) interface/stm32f103_driver_w25qxx_interface.c src/driver_$(TARGET).c -o $(BUILD_DIR)/$(TARGET).o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
