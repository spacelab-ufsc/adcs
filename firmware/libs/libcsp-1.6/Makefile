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

CFLAGS=--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -I../../config/ -Iinclude/ -I../../os/include/ -I../../os/portable/GCC/ARM_CM3/ -mcpu=cortex-m3 -mthumb -msoft-float -c

.PHONY: all
all: csp_bridge.o csp_buffer.o csp_conn.o csp_crc32.o csp_debug.o csp_dedup.o csp_endian.o csp_hex_dump.o csp_iflist.o csp_init.o csp_io.o csp_port.o csp_promisc.o csp_qfifo.o csp_route.o csp_service_handler.o csp_services.o csp_sfp.o csp_system.o csp_time.o csp_clock.o csp_malloc.o csp_queue.o csp_semaphore.o csp_system_frtos.o csp_thread.o csp_time_frtos.o csp_hmac.o csp_sha1.o csp_xtea.o csp_if_can.o csp_if_lo.o csp_rtable.o csp_udp.o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o

csp_bridge.o: src/csp_bridge.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_buffer.o: src/csp_buffer.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_conn.o: src/csp_conn.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_crc32.o: src/csp_crc32.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_debug.o: src/csp_debug.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_dedup.o: src/csp_dedup.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_endian.o: src/csp_endian.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_hex_dump.o: src/csp_hex_dump.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_iflist.o: src/csp_iflist.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_init.o: src/csp_init.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_io.o: src/csp_io.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_port.o: src/csp_port.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_promisc.o: src/csp_promisc.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_qfifo.o: src/csp_qfifo.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_route.o: src/csp_route.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_service_handler.o: src/csp_service_handler.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_services.o: src/csp_services.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_sfp.o: src/csp_sfp.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_system.o: src/arch/csp_system.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_time.o: src/arch/csp_time.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_clock.o: src/arch/freertos/csp_clock.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_malloc.o: src/arch/freertos/csp_malloc.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_queue.o: src/arch/freertos/csp_queue.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_semaphore.o: src/arch/freertos/csp_semaphore.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_system_frtos.o: src/arch/freertos/csp_system.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_thread.o: src/arch/freertos/csp_thread.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_time_frtos.o: src/arch/freertos/csp_time.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_hmac.o: src/crypto/csp_hmac.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_sha1.o: src/crypto/csp_sha1.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_xtea.o: src/crypto/csp_xtea.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_if_can.o: src/interfaces/csp_if_can.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_if_lo.o: src/interfaces/csp_if_lo.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_rtable.o: src/rtable/csp_rtable.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

csp_udp.o: src/transport/csp_udp.c
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@
