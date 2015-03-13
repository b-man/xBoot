SRC_DIR := $(SRCROOT)/lib/crt/ctype

C_SRC_FILES := \
	isacii.c \
	isalpha.c \
	isdigit.c \
	isspace.c \
	isupper.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
