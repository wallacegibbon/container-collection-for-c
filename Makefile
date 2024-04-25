TARGET = container_collection

#INSTALL_DIR = /home/wallace/lib
INSTALL_DIR = C:/lib

C_SOURCE_FILES += $(wildcard ./src/*.c)
C_INCLUDES += ./src ./include

#COMMON_C_FLAGS += -DNO_MALLOC

AR = ar

.PHONY: install

include ./cc-with-test.mk

$(BUILD_DIR)/lib$(TARGET).a: $(OBJECTS)
	@$(AR) -rcs $@ $^

install: $(BUILD_DIR)/lib$(TARGET).a
	@mkdir -p $(INSTALL_DIR)/$(TARGET)/{lib,include}
	@cp -r include/* $(INSTALL_DIR)/$(TARGET)/include/
	@cp $(BUILD_DIR)/lib$(TARGET).a $(INSTALL_DIR)/$(TARGET)/lib/
	@echo
	@echo -e "\tlibrary \"$(TARGET)\" has been installed. (Succeed!)"
	@echo

