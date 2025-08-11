TARGET=containers

BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

PARTNUMBER=stm32f103c8t6

CC=arm-none-eabi-gcc
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
all: queue_lib.o buffer_lib.o

queue_lib.o: queue.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

buffer_lib.o: buffer.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
