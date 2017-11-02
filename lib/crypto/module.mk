SRC_DIR := $(SRCROOT)/lib/crypto

C_SRC_FILES := \
	sha1.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
