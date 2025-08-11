TARGET ?= libpredict
BUILD_DIR ?= $(CURDIR)
BUILD_DIR_SRC = $(BUILD_DIR)/$(TARGET)

TOOLCHAIN_PREFIX ?=
CC_FLAGS_APPEND ?=

CC := $(TOOLCHAIN_PREFIX)gcc
INC := ../include/
FLAGS := -fpic -std=gnu99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -lm -I$(INC) -O3

FLAGS += $(CC_FLAGS_APPEND)

.PHONY: all
all: $(BUILD_DIR_SRC)/julian_date.o $(BUILD_DIR_SRC)/moon.o $(BUILD_DIR_SRC)/observer.o $(BUILD_DIR_SRC)/orbit.o $(BUILD_DIR_SRC)/refraction.o $(BUILD_DIR_SRC)/sdp4.o $(BUILD_DIR_SRC)/sgp4.o $(BUILD_DIR_SRC)/sun.o $(BUILD_DIR_SRC)/unsorted.o

$(BUILD_DIR_SRC)/julian_date.o: julian_date.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/moon.o: moon.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/observer.o: observer.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/orbit.o: orbit.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/refraction.o: refraction.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/sdp4.o: sdp4.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/sgp4.o: sgp4.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/sun.o: sun.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR_SRC)/unsorted.o: unsorted.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR_SRC)/*.o
