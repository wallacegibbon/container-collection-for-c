OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCE_FILES:.c=.c.o)))

C_FLAGS += -g -Wall -Wextra -Wno-unused -ffunction-sections -fdata-sections \
-Wp,-MMD,-MT"$@",-MF"$(@:.o=.d)",-MP \
$(addprefix -I, $(C_INCLUDES))

LD_FLAGS += -Wl,--gc-sections,-Map=$@.map

ifeq ($(MEMCHECK), 1)
MEMORY_CHECK_PROG = valgrind --leak-check=full --error-exitcode=1
else ifeq ($(MEMCHECK), 2)
C_FLAGS += -fno-inline -fno-omit-frame-pointer
LD_FLAGS += -static-libgcc
MEMORY_CHECK_PROG = drmemory -batch -suppress \
	$(HOME)/drmemory-mingw64-suppress.txt -exit_code_if_errors 1 --
endif

TARGET ?= target
BUILD_DIR ?= build
INSTALL_DIR ?= $(HOME)/.local

CC = cc
AR = ar

.PHONY: all build_dir clean install

vpath %.c $(sort $(dir $(C_SOURCE_FILES)))

all: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c | build_dir
	@echo "CC $<"
	@$(CC) -c -o $@ $< $(C_FLAGS)

$(BUILD_DIR)/lib$(TARGET).a: $(OBJECTS)
	@echo "AR $@"
	@$(AR) -rcsv $@ $^

install: $(BUILD_DIR)/lib$(TARGET).a
	@mkdir -p $(INSTALL_DIR)/lib
	@mkdir -p $(INSTALL_DIR)/include
	@echo "CP include/* $(INSTALL_DIR)/include/"
	@cp -r include/* $(INSTALL_DIR)/include/
	@echo "CP $(BUILD_DIR)/lib$(TARGET).a $(INSTALL_DIR)/lib/"
	@cp $(BUILD_DIR)/lib$(TARGET).a $(INSTALL_DIR)/lib/

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

vpath %.c ./test

$(BUILD_DIR)/%: %.c $(OBJECTS) | build_dir
	@echo "CC $<"
	@$(CC) -o $@ $^ $(C_FLAGS) $(LD_FLAGS)
	@echo "./$@\n"
	@$(MEMORY_CHECK_PROG) $@
	@rm $@

-include $(OBJECTS:.o=.d)

