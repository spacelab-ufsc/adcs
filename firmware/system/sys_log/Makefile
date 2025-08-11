ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
READELF=arm-none-eabi-readelf
NM=arm-none-eabi-nm
SIZE=arm-none-eabi-size
GDB=arm-none-eabi-gdb

CFLAGS=-c -mcpu=cortex-m3 -mthumb -O0 -I../../ -I../../config/ -I../../os/include/ -I../../os/portable/GCC/ARM_CM3/

.PHONY: all
all: sys_log.o sys_log_mutex.o sys_log_uart.o

sys_log.o: sys_log.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

sys_log_mutex.o: sys_log_mutex.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

sys_log_uart.o: sys_log_uart.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
