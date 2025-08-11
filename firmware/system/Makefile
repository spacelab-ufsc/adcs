ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

ifndef OPENCM3_FAMILY
	OPENCM3_FAMILY=STM32F1
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

CFLAGS=-c -mcpu=cortex-m3 -mthumb -O0 -I../ -I../hal/include -I../config/ -I../os/include/ -I../os/portable/GCC/ARM_CM3/ -D$(OPENCM3_FAMILY)

.PHONY: all
all: hooks_sys.o system_sys.o sys_log_rule

hooks_sys.o: hooks.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

system_sys.o: system.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

sys_log_rule: sys_log/Makefile
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C sys_log

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
