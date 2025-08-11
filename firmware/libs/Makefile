BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

.PHONY: all
all:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C containers
	#$(MAKE) BUILD_DIR=$(BUILD_DIR) -C libcsp-1.6
	$(MAKE) BUILD_DIR=$(BUILD_DIR) TOOLCHAIN_PREFIX=arm-none-eabi- CC_FLAGS_APPEND="--static -nostartfiles -g3 -Os -fno-common -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -msoft-float -c" -C libpredict

.PHONY: clean
clean:
	$(MAKE) clean -C libpredict
