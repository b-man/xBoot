# xBoot BSP for ST-Ericsson Snowball

ARCH            := arm
SUBARCH         := cortex-a9
PLATFORM        := snowball
DRAM_BASE       := 0x00000000
DRAM_SIZE       := 0x40000000
BOOT_TEXT_BASE  := 0x00100000

SRC_DIR := $(SRCROOT)/bsp/snowball

C_SRC_FILES := \
	config.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))

include $(SRCROOT)/driver/timer/ste_mtu/module.mk
include $(SRCROOT)/driver/serial/pl011/module.mk
