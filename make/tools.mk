##
# Tools used for building xBoot
##

CROSS	:= arm-none-eabi-
CC	:= $(CROSS)gcc
LD	:= $(CROSS)ld
AS	:= $(CROSS)as
AR	:= $(CROSS)ar
OBJDMP	:= $(CROSS)objdump
OBJCPY	:= $(CROSS)objcopy
HOST_CC	:= gcc

##
# Used for quiet vs verbose building - do not touch!
#

ifndef VERBOSE_BUILD
	_CC := @$(SRCROOT)/scripts/ccdv $(CC)
	_AS := @$(SRCROOT)/scripts/ccdv $(AS)
	_AR := @$(SRCROOT)/$(SRCROOT)scripts/ccdv $(AR)
	_LD := @$(SRCROOT)/scripts/ccdv $(LD)
	_OBJCPY := @$(SRCROOT)/scripts/ccdv $(OBJCPY)
	_OBJDMP := @$(SRCROOT)/scripts/ccdv $(OBJDMP)
else
	_CC := $(CC)
	_AS := $(AS)
	_AR := $(AR)
	_LD := $(LD)
	_OBJCPY := $(OBJCPY)
	_OBJDMP := $(OBJDMP)
endif
