SRC_DIR := $(SRCROOT)/lib/crt/stdlib

C_SRC_FILES := \
	env.c \
	malloc.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
