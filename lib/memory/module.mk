SRC_DIR := $(SRCROOT)/lib/memory

C_SRC_FILES := \
	zalloc.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
