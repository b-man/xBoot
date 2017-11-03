SRC_DIR := $(SRCROOT)/lib/boot

C_SRC_FILES := \
	aslr.c \
	boot.c \
	dtre.c \
	jsdt.c \
	device_tree.c  \
	image3.c  \
	jsmn.c  \
	macho_loader.c  \
	memory_region.c

# needed for image3.c
CFLAGS += -Wno-multichar

C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
