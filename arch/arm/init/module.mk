SRC_DIR := $(SRCROOT)/arch/arm/init

S_SRC_FILES := \
	start.S \
	vectors.S

C_SRC_FILES := \
	traps.c \
	init.c

SFLAGS += -I$(SRC_DIR)/../include
CFLAGS += -I$(SRC_DIR)/../include

S_SRCS += $(addprefix $(SRC_DIR)/, $(S_SRC_FILES))
C_SRCS += $(addprefix $(SRC_DIR)/, $(C_SRC_FILES))
