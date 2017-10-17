# Project name and version
export RC_ProjectName := xBoot
export RC_ProjectSourceVersion := 1
export RC_ProjectBuildVersion := 0

export SRCROOT := $(CURDIR)
export OBJROOT := $(SRCROOT)

# Main build target
export TARGET := xboot.$(PLATFORM)

include $(SRCROOT)/make/vars.mk
include $(SRCROOT)/make/tools.mk
ifdef PLATFORM
        BSP_PATH = $(SRCROOT)/bsp/$(PLATFORM)/bsp.mk
        ifneq ("$(wildcard $(BSP_PATH))","")
                include $(BSP_PATH)
        else
                $(error Unsupported platform "$(PLATFORM)" specified. Run "make help" for a list of supported platforms.)
        endif
endif
include $(SRCROOT)/make/dirs.mk
include $(SRCROOT)/make/flags.mk
include $(SRCROOT)/make/build.mk

export DERIVED_SOURCES_DIR := $(BUILD_GEN)

include $(SRCROOT)/make/rules.mk
