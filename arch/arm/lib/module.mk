SRC_DIR := $(SRCROOT)/arch/arm/lib

S_SRC_FILES := \
	string/bcopy.S \
	string/bzero.S \
	string/strchr.S \
	string/strcmp.S \
	string/strlen.S \
	string/strncmp.S \
	string/memset_pattern.S

S_SRCS += $(addprefix $(SRC_DIR)/, $(S_SRC_FILES))
