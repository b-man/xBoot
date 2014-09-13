##
# Common make rules for building xBoot
##

##
# C source code rules
#

%.o: %.c
	$(eval obj_uuid := $(shell uuidgen -r))
	$(_CC) $(CFLAGS) -c $< -o $(BUILD_OBJS)/$(notdir $*-$(obj_uuid).o)
	@echo "$(BUILD_OBJS)/$(notdir $*-$(obj_uuid).o)" >> $(BUILD_OBJS)/Objects.list

##
# ASM source code rules
#

%.o: %.S
	$(eval obj_uuid := $(shell uuidgen -r))
	$(_CC) -x assembler-with-cpp $(SFLAGS) -c $< -o $(BUILD_OBJS)/$(notdir $*-$(obj_uuid).o)
	@echo "$(BUILD_OBJS)/$(notdir $*-$(obj_uuid).o)" >> $(BUILD_OBJS)/Objects.list

##
# Main rules
#

.PHONY: all install clean help
.SECONDARY: $(TARGET)

all: setup $(TARGET)

setup:
	@if [ -z $(PLATFORM) ]; then												\
		echo "Error: no platform specified.\n";										\
		echo "Use PLATFORM= to specify a platform or run \"make help\" to get a list of supported platforms.\n";	\
		exit 1;														\
	fi;
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_GEN)
	@mkdir -p $(BUILD_OBJS)
	@$(HOST_CC) -O1 $(SRCROOT)/scripts/ccdv.c -o $(SRCROOT)/scripts/ccdv
	@$(SRCROOT)/scripts/version.pl $(RC_ProjectName) > $(BUILD_GEN)/$(RC_ProjectName)_version.c
	$(_CC) $(CFLAGS) -c $(BUILD_GEN)/$(RC_ProjectName)_version.c -o $(BUILD_ROOT)/$(RC_ProjectName)_version.o

$(TARGET): $(OBJS)
	@bash -e $(SRCROOT)/scripts/archive.sh $(AR) $(BUILD_OBJS)/Objects.list $(BUILD_DIR)/$(TARGET).a
	$(_LD) -T $(SRCROOT)/arch/arm/init/xboot.ld $(BUILD_DIR)/$(TARGET).a $(BUILD_ROOT)/$(RC_ProjectName)_version.o -o  $(BUILD_DIR)/$(TARGET).elf $(LDFLAGS)
	$(_OBJDMP) -D $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).list
	$(_OBJCPY) $(BUILD_DIR)/$(TARGET).elf -g -S -O binary $(SRCROOT)/$(TARGET).bin
	@echo "$(TARGET).bin is ready."

install: all

clean:
	@rm -rf $(BUILD_DIR) $(SRCROOT)/$(TARGET).* $(SRCROOT)/scripts/ccdv
	@echo "Source tree is now clean."


help:
	@echo "When building xBoot, you must set PLATFORM to to a desired platform target.\n"
	@echo -n "Platforms supported by xBoot: "
	@for i in `ls $(SRCROOT)/configs/ | cut -d '.' -f1`; do \
		echo -n "$$i ";			\
	done;
	@echo "\n\nIf you wish to see commands being called while building, you can also set VERBOSE_BOOT to YES\n"
