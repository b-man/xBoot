SRC_DIR := $(SRCROOT)/lib/crt/string

C_SRC_FILES := \
	atoi.c \
	hextoul.c \
	strchr.c \
	strcmp.c \
	strcpy.c \
	strdup.c \
	strlen.c \
	strncmp.c \
	strncpy.c \
	strtok.c \
	strtoul.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
