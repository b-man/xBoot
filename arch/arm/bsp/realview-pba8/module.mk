SRC_DIR := $(SRCROOT)/arch/arm/bsp/realview-pba8

C_SRC_FILES := \
	config.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
