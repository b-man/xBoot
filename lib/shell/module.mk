SRC_DIR := $(SRCROOT)/lib/shell

C_SRC_FILES := \
	cmds.c \
	shell.c \
	history.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))

include $(SRCROOT)/lib/shell/cmds/module.mk
