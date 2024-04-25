BUILD_DIR ?= build

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

#COMMON_C_FLAGS += -W -g -ffunction-sections -fdata-sections -MMD -MP $(addprefix -I, $(C_INCLUDES))
COMMON_C_FLAGS += -W -g -MMD -MP $(addprefix -I, $(C_INCLUDES)) -Wno-unused-parameter

COMMON_LD_FLAGS += -Wl,--gc-sections -Wl,-Map=$@.map

ifeq ($(MEMCHECK), 1)
COMMON_C_FLAGS += -fno-inline -fno-omit-frame-pointer
COMMON_LD_FLAGS += -static-libgcc
MEMORY_CHECK_PROG = drmemory --
endif

CC = cc

.PHONY: all build_dir clean

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@echo -e "\tCC $<"
	@$(CC) -c -o $@ $< $(COMMON_C_FLAGS)

vpath %.c ./test

$(BUILD_DIR)/%: %.c $(OBJECTS) | build_dir
	@echo -e "\tCC $<"
	@$(CC) -o $@ $^ $(COMMON_C_FLAGS) $(COMMON_LD_FLAGS)
	@echo -e "\t./$@\n"
	@$(MEMORY_CHECK_PROG) $@

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

-include $(OBJECTS:.o=.d)

