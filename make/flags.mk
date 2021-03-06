##
# Common toolchain flags used for building xBoot
##

##
# Common C compiler flags
#

CFLAGS_ARGS	+= \
	-mcpu=$(SUBARCH) \
	-fno-builtin \
	-fPIC \
	-nostdlib \
	-nostartfiles \
	-ffreestanding \
	-std=c11 \

ifeq ($(BUILD),DEBUG)
	CFLAGS_ARGS	+= \
		-g \
		-O0
else
	CFLAG_ARGS	+= \
		-Os
endif

CFLAGS_DEFS	+= \
	-D__LITTLE_ENDIAN__=1 \
	-DDRAM_BASE=$(DRAM_BASE) \
	-DDRAM_SIZE=$(DRAM_SIZE) \
	-DBUILD_STYLE=\"$(BUILD)\"

ifeq ($(SUBARCH),cortex-a9)
	CFLAGS_DEFS	+= \
		-DCORTEX_A9 \
		-DUSE_4K_PAGES
endif

ifeq ($(SUBARCH),cortex-a8)
	CFLAGS_DEFS	+= \
		-DCORTEX_A8 \
		-DUSE_4K_PAGES
endif

ifeq ($(SUBARCH),cortex-a7)
	CFLAGS_DEFS	+= \
		-DCORTEX_A8 \
		-DUSE_4K_PAGES
endif

CFLAGS_WARNS	+= \
	-Wall \
	-Werror \
	-Wno-error=multichar \
	-Wno-unused-value

CFLAGS_INCS	+= \
	-I$(SRCROOT) \
	-I$(SRCROOT)/include \
	-I$(SRCROOT)/arch/$(ARCH)/include

# Set everything up
CFLAGS += $(CFLAGS_ARGS)
CFLAGS += $(CFLAGS_DEFS)
CFLAGS += $(CFLAGS_WARNS)
CFLAGS += $(CFLAGS_INCS)

##
# Common assembler flags
#

SFLAGS_ARGS	+= \
	$(CFLAGS_ARGS)

SFLAGS_DEFS	+= \
	$(CFLAGS_DEFS) \
	-D__ASSEMBLY__=1

SFLAGS_WARNS	+= \
	$(CFLAGS_WARNS)

SFLAGS_INCS	+= \
	$(CFLAGS_INCS)

# Set everything up
SFLAGS += $(SFLAGS_ARGS)
SFLAGS += $(SFLAGS_DEFS)
SFLAGS += $(SFLAGS_WARNS)
SFLAGS += $(SFLAGS_INCS)

##
# Common linker flags
#

LDFLAGS	+= \
	-static \
	-nostdlib \
	--gc-sections \
	--defsym DRAM_SIZE=$(DRAM_SIZE) \
	--defsym BOOT_TEXT_BASE=$(BOOT_TEXT_BASE) \
	-L$(BUILD_ROOT)
