SRC_DIR := $(SRCROOT)/lib/shell/cmds

C_SRC_FILES := \
	help.c \
	halt.c \
	boot.c \
	reset.c \
	getenv.c \
	setenv.c \
	printenv.c \
	history.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
