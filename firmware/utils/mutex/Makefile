TARGET=mutex

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

ifndef OPENCM3_FAMILY
	OPENCM3_FAMILY=STM32F1
endif

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

LIBS= -I../../ -I../../hal/include/ -I../../os/include/ -I../../config/ -I../../os/portable/GCC/ARM_CM3/ 
CFLAGS=-c -mcpu=cortex-m3 -mthumb -Os $(LIBS) -D$(OPENCM3_FAMILY)

.PHONY: all
all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(BUILD_DIR)/$(TARGET).o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
