SRC_DIR := $(SRCROOT)/driver/uart/pl011

C_SRC_FILES := \
	pl011.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
