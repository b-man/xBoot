# Project name and version
export RC_ProjectName := xBoot
export RC_ProjectSourceVersion := 57
export RC_ProjectBuildVersion := 2

export SRCROOT := $(CURDIR)
export OBJROOT := $(SRCROOT)

# Default build style
export BUILD ?= DEBUG

# Main build target
export TARGET := xboot.$(PLATFORM).$(BUILD)

include $(SRCROOT)/make/vars.mk
include $(SRCROOT)/make/tools.mk
ifdef PLATFORM
	BSP_DIR := $(SRCROOT)/bsp/$(PLATFORM)
        BSP_MKFILE := $(BSP_DIR)/bsp.mk
        ifneq ("$(wildcard $(BSP_MKFILE))","")
                include $(BSP_MKFILE)
        else
                $(error Unsupported platform "$(PLATFORM)" specified. Run "make help" for a list of supported platforms.)
        endif
endif
include $(SRCROOT)/make/dirs.mk
include $(SRCROOT)/make/build.mk
include $(SRCROOT)/make/flags.mk

export DERIVED_SOURCES_DIR := $(BUILD_GEN)

include $(SRCROOT)/make/rules.mk
