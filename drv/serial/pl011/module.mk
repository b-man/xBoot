SRC_DIR := $(SRCROOT)/drv/serial/pl011

C_SRC_FILES := \
	pl011.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
