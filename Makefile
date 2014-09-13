# Project name and version
export RC_ProjectName := xBoot
export RC_ProjectSourceVersion := 1
export RC_ProjectBuildVersion := 0

export SRCROOT := $(CURDIR)
export OBJROOT := $(SRCROOT)

# Main build target
export TARGET := xboot

include $(SRCROOT)/make/vars.mk
include $(SRCROOT)/make/tools.mk
ifdef PLATFORM
	include $(SRCROOT)/configs/$(PLATFORM).mk
endif
include $(SRCROOT)/make/flags.mk
include $(SRCROOT)/make/dirs.mk
include $(SRCROOT)/make/drvs.mk
include $(SRCROOT)/make/build.mk

export DERIVED_SOURCES_DIR := $(BUILD_GEN)

include $(SRCROOT)/make/rules.mk
