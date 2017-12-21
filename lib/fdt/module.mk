LIBFDT_DIR := $(SRCROOT)/tools/fdt/dtc/libfdt

CFLAGS += -I$(LIBFDT_DIR)

C_SRC_FILES := \
	fdt_addresses.c \
	fdt.c \
	fdt_empty_tree.c \
	fdt_overlay.c \
	fdt_ro.c \
	fdt_rw.c \
	fdt_strerror.c \
	fdt_sw.c \
	fdt_wip.c

C_SRCS += $(addprefix $(LIBFDT_DIR)/, $(C_SRC_FILES))
