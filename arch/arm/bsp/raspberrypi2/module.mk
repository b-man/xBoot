SRC_DIR := $(SRCROOT)/arch/arm/bsp/raspberrypi2

C_SRC_FILES := \
	config.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
