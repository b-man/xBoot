# Main make inclusions for ARM support

include $(SRCROOT)/arch/arm/lib/module.mk
include $(SRCROOT)/arch/arm/init/module.mk
include $(SRCROOT)/arch/arm/bsp/$(PLATFORM)/module.mk