# xBoot BSP for the Raspberry Pi 2 Board

ARCH            := arm
SUBARCH         := cortex-a7
PLATFORM        := raspberrypi
DRAM_BASE       := 0x00008000
DRAM_SIZE       := 0x10000000
BOOT_TEXT_BASE  := 0x00008000

SRC_DIR := $(SRCROOT)/bsp/raspberrypi2

C_SRC_FILES := \
	config.c

PLAT_DT_FILE := \
	bcm2836.jsdt

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))

include $(SRCROOT)/driver/timer/bcm_timer/module.mk
include $(SRCROOT)/driver/uart/pl011/module.mk
