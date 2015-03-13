# xBoot configuration for the Raspberry Pi 2 Board

ARCH		:= arm
SUBARCH		:= cortex-a7
PLATFORM	:= raspberrypi
DRAM_BASE	:= 0x00000000
DRAM_SIZE	:= 0x40000000
BOOT_TEXT_BASE	:= 0x00008000

PL011_UART	:= YES
BCM_TIMER	:= YES
