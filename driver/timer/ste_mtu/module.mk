SRC_DIR := $(SRCROOT)/driver/timer/ste_mtu

C_SRC_FILES := \
	ste_mtu.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
