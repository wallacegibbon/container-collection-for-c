TARGET = container_collection

C_SOURCE_FILES += $(wildcard ./src/*.c)

C_INCLUDES += ./src ./include

#C_FLAGS += -DNO_MALLOC

include ./cc-with-test.mk

