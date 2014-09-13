##
# Common build settings for xBoot build
##

##
# Set build directory
#

BUILD_DIR	+= $(SRCROOT)/Build
BUILD_ROOT	+= $(BUILD_DIR)
BUILD_OBJS	+= $(BUILD_ROOT)/Objects
BUILD_GEN	+= $(BUILD_ROOT)/GeneratedSources

##
# Import all subdirectory source trees
#

ifdef PLATFORM
	include $(patsubst %,%/module.mk,$(DIRS))
endif

##
# Create a list of objects to compile
#

OBJS += \
	$(patsubst %.S,%.o, $(filter %.S,$(S_SRCS))) \
	$(patsubst %.c,%.o, $(filter %.c,$(C_SRCS)))
