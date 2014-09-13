SRC_DIR := $(SRCROOT)/lib/crt/string

C_SRC_FILES := \
	atoi.c \
	strtok.c \
	strncpy.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
