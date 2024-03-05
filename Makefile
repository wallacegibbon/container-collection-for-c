BUILD_DIR ?= build

C_SOURCE_FILES += $(wildcard ./src/*.c)
C_INCLUDES += ./src

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

C_FLAGS += -W -g -MMD -MP -MF"$(@:%.o=%.d)" $(addprefix -I, $(C_INCLUDES))

CC = gcc

.PHONY: all build_dir clean test_char test_float test_char_sort test_struct_sort

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	$(CC) -c -o $@ $< $(C_FLAGS)

test_char: test/test_char.c $(OBJECTS)
	$(CC) -o $(BUILD_DIR)/$@.elf $^
	$(BUILD_DIR)/$@.elf

test_float: test/test_float.c $(OBJECTS)
	$(CC) -o $(BUILD_DIR)/$@.elf $^
	$(BUILD_DIR)/$@.elf

test_char_sort: test/test_char_sort.c $(OBJECTS)
	$(CC) -o $(BUILD_DIR)/$@.elf $^
	$(BUILD_DIR)/$@.elf

test_struct_sort: test/test_struct_sort.c $(OBJECTS)
	$(CC) -o $(BUILD_DIR)/$@.elf $^
	$(BUILD_DIR)/$@.elf

build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

