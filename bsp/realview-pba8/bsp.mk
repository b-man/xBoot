# xBoot BSP for the ARM RealView Platform Board

ARCH            := arm
SUBARCH         := cortex-a8
PLATFORM        := realview-pb
DRAM_BASE       := 0x70000000
DRAM_SIZE       := 0x20000000
BOOT_TEXT_BASE  := 0x80000040

SRC_DIR := $(SRCROOT)/bsp/realview-pba8

C_SRC_FILES := \
	config.c

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))

include $(SRCROOT)/driver/timer/sp804/module.mk
include $(SRCROOT)/driver/uart/pl011/module.mk
