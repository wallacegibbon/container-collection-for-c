## To use drmemory, you need to compile the target with special flags.
## Use the DEBUG variable to control the build process. (run `make DEBUG=1`)

BUILD_DIR ?= build

C_SOURCE_FILES += $(wildcard ./src/*.c)
C_INCLUDES += ./src ./include

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

COMMON_C_FLAGS += -W -g $(addprefix -I, $(C_INCLUDES))
#COMMON_C_FLAGS += -DNO_MALLOC

ifeq ($(DEBUG), 1)
COMMON_C_FLAGS += -fno-inline -fno-omit-frame-pointer
MEMORY_CHECK_PROG = drmemory --
DEBUG_SIGN = (debugging build)
endif

C_FLAGS += $(COMMON_C_FLAGS) -MMD -MP -MF"$(@:%.o=%.d)"

TEST_C_FLAGS += $(COMMON_C_FLAGS)

ifeq ($(DEBUG), 1)
TEST_C_FLAGS += -static-libgcc -static-libstdc++ -ggdb
endif

CC = gcc

.PHONY: all build_dir clean

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@echo -e "\tcompiling $(DEBUG_SIGN) $< ..."
	@$(CC) -c -o $@ $< $(C_FLAGS)

vpath %.c ./test

$(BUILD_DIR)/%: %.c $(OBJECTS)
	@echo -e "\tcompiling $(DEBUG_SIGN) $< ..."
	@$(CC) -o $@ $^ $(TEST_C_FLAGS)
	@echo -e "\trunning test $@ ..."
	@$(MEMORY_CHECK_PROG) $@

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

