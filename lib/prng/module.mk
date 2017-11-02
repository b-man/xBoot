SRC_DIR := $(SRCROOT)/lib/prng

C_SRC_FILES := \
	prng.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
