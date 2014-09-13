SRC_DIR := $(SRCROOT)/lib/crt/stdio

C_SRC_FILES := \
	printf.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
