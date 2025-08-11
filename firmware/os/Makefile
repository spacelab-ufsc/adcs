BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

CFLAGS=--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -I../config/ -Iinclude/ -Iportable/GCC/ARM_CM3/ -mcpu=cortex-m3 -mthumb -msoft-float -c

.PHONY: all
all: croutine_frtos.o event_groups_frtos.o list_frtos.o queue_frtos.o stream_buffer_frtos.o tasks_frtos.o timers_frtos.o heap_1_frtos.o port_frtos.o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o

croutine_frtos.o: croutine.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

event_groups_frtos.o: event_groups.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

list_frtos.o: list.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

queue_frtos.o: queue.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

stream_buffer_frtos.o: stream_buffer.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

tasks_frtos.o: tasks.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

timers_frtos.o: timers.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

heap_1_frtos.o: portable/MemMang/heap_1.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

port_frtos.o: portable/GCC/ARM_CM3/port.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@
