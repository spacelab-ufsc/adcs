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

CFLAGS=-c -mcpu=cortex-m3 -mthumb -O0 -I../ -I../hal/include/ -I../config/ -I../os/include/ -I../os/portable/GCC/ARM_CM3/  -D$(OPENCM3_FAMILY)

.PHONY: all
all: heartbeat_tks.o startup_tks.o tasks_tks.o

heartbeat_tks.o: heartbeat.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

startup_tks.o: startup.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

tasks_tks.o: tasks.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
