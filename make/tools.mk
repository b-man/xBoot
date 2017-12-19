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

CCDV	:= $(SRCROOT)/tools/ccdv/ccdv
VERSION	:= $(SRCROOT)/tools/scripts/version.pl
ARCHIVE := $(SRCROOT)/tools/scripts/archive.sh
IMAGE3MAKER := $(SRCROOT)/tools/image3maker/image3maker

##
# Used for quiet vs verbose building - do not touch!
#

ifneq ($(VERBOSE_BUILD),1)
	_CC := @$(CCDV) $(CC)
	_AS := @$(CCDV) $(AS)
	_AR := @$(CCDV) $(AR)
	_LD := @$(CCDV) $(LD)
	_OBJCPY := @$(CCDV) $(OBJCPY)
	_OBJDMP := @$(CCDV) $(OBJDMP)
else
	_CC := $(CC)
	_AS := $(AS)
	_AR := $(AR)
	_LD := $(LD)
	_OBJCPY := $(OBJCPY)
	_OBJDMP := $(OBJDMP)
endif
