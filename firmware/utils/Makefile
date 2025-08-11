BUILD_DIR ?= $(CURDIR)/build

BUILD_DIR_ABS = $(abspath $(BUILD_DIR))

.PHONY: all
all:
	$(MAKE) BUILD_DIR=$(BUILD_DIR) -C mutex

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o
