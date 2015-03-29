# xBoot BSP for the Raspberry Pi 2 Board

ARCH            := arm
SUBARCH         := cortex-a8
PLATFORM        := raspberrypi
DRAM_BASE       := 0xC0000000
DRAM_SIZE       := 0x40000000
BOOT_TEXT_BASE  := 0x00008000

SRC_DIR := $(SRCROOT)/bsp/raspberrypi2

C_SRC_FILES := \
	config.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))

include $(SRCROOT)/driver/timer/bcm_timer/module.mk
include $(SRCROOT)/driver/serial/pl011/module.mk


