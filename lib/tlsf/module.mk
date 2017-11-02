SRC_DIR := $(SRCROOT)/lib/tlsf

C_SRC_FILES := \
	tlsf.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
