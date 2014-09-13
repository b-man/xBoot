# xBoot configuration for ST-Ericsson Snowball

ARCH		:= arm
SUBARCH		:= cortex-a9
PLATFORM	:= snowball
DRAM_BASE	:= 0x00000000
DRAM_SIZE	:= 0x40000000
BOOT_TEXT_BASE	:= 0x00100000

PL011_UART	:= YES
STE_MTU_TIMER	:= YES
