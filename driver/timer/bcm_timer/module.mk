SRC_DIR := $(SRCROOT)/driver/timer/bcm_timer

C_SRC_FILES := \
	bcm_timer.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
