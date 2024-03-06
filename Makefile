BUILD_DIR ?= build

C_SOURCE_FILES += $(wildcard ./src/*.c)
C_INCLUDES += ./src

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

COMMON_C_FLAGS += -W -g $(addprefix -I, $(C_INCLUDES))
#COMMON_C_FLAGS += -DNO_MALLOC

C_FLAGS += $(COMMON_C_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)"

TEST_C_FLAGS += $(COMMON_C_FLAGS)

CC = gcc

.PHONY: all build_dir clean

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@echo -e "\tcompiling $< ..."
	@$(CC) -c -o $@ $< $(C_FLAGS)

vpath %.c ./test

$(BUILD_DIR)/%: %.c $(OBJECTS)
	@echo -e "\tcompiling $< ..."
	@$(CC) -o $@ $^ $(TEST_C_FLAGS)
	@echo -e "\trunning test $@ ..."
	@$@

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

