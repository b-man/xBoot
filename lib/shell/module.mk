SRC_DIR := $(SRCROOT)/lib/shell

C_SRC_FILES := \
	cmds.c \
	parser.c \
	cmds/help.c \
	cmds/halt.c \
	cmds/reset.c \
	cmds/getenv.c \
	cmds/setenv.c \
	cmds/printenv.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
