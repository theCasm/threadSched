TARGET := libthreads.so

BUILD_DIR := ./build
SRC_DIR := src

CC := clang
ASM := nasm

CFLAGS := -Wall -fPIC
ASMFLAGS := -felf64
LDFLAGS := -shared

SRCS := $(shell find $(SRC_DIR) -name '*.c' -or -name '*.asm')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@ 

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
