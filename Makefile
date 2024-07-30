TARGET = container_collection

C_SOURCE_FILES += $(wildcard ./src/*.c)

C_INCLUDES += ./src ./include

include ./cc-with-test.mk

