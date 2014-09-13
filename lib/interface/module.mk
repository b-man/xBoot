SRC_DIR := $(SRCROOT)/lib/interface

C_SRC_FILES := \
	serial.c \
	sysctl.c \
	timer.c \
	nvram.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
