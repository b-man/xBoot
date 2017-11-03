SRC_DIR := $(SRCROOT)/arch/arm/lib

S_SRC_FILES := \
	string/bcopy.S \
	string/bzero.S \
	string/memcpy.S \
	string/memmove.S \
	string/memset.S \
	builtin/div.S

S_SRCS += $(addprefix $(SRC_DIR)/, $(S_SRC_FILES))
