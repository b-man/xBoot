SRC_DIR := $(SRCROOT)/driver/timer/sp804

C_SRC_FILES := \
	sp804.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
