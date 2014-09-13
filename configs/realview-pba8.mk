# xBoot configuration for the ARM RealView Platform Board

ARCH		:= arm
SUBARCH		:= cortex-a8
PLATFORM	:= realview-pb
DRAM_BASE	:= 0x70000000
DRAM_SIZE	:= 0x20000000
BOOT_TEXT_BASE	:= 0x80000040

PL011_UART	:= YES
SP804_TIMER := YES
