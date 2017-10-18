SRC_DIR := $(SRCROOT)/lib/device

C_SRC_FILES := \
	uart.c \
	sysctl.c \
	timer.c \
	nvram.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
