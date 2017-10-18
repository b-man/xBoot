##
# Source tree setup for xBoot build
##

##
# Define basic source tree that makes up xBoot
#

DIRS += \
	core \
	lib/crt \
	lib/boot \
	lib/shell \
	lib/device \
	lib/debug \
	lib/memory \
	arch/$(ARCH)
