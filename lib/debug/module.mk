SRC_DIR := $(SRCROOT)/lib/debug

C_SRC_FILES := \
	panic.c \
	assert.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))