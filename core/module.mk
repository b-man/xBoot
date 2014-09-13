SRC_DIR := $(SRCROOT)/core

C_SRC_FILES := \
	main.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
