##
# Driver imports for xBoot
##

# Update this file for every new driver added to xBoot

ifeq ($(PL011_UART),YES)
	DIRS += $(SRCROOT)/drv/serial/pl011
endif

ifeq ($(STE_MTU_TIMER),YES)
	DIRS += $(SRCROOT)/drv/timer/ste_mtu
endif

ifeq ($(SP804_TIMER),YES)
	DIRS += $(SRCROOT)/drv/timer/sp804
endif

ifeq ($(BCM_TIMER),YES)
	DIRS += $(SRCROOT)/drv/timer/bcm_timer
endif
