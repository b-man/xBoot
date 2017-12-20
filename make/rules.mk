##
# Common make rules for building xBoot
##

##
# C source code rules
#

%.o: %.c
	$(eval obj_uuid := $(shell uuidgen -r | cut -d'-' -f1))
	$(_CC) $(CFLAGS) -c $< -o $(BUILD_OBJS)/$(notdir $*.$(obj_uuid).o)
	@echo "$(BUILD_OBJS)/$(notdir $*.$(obj_uuid).o)" >> $(BUILD_OBJS)/Objects.list

##
# ASM source code rules
#

%.o: %.S
	$(eval obj_uuid := $(shell uuidgen -r | cut -d'-' -f1))
	$(_CC) -x assembler-with-cpp $(SFLAGS) -c $< -o $(BUILD_OBJS)/$(notdir $*.$(obj_uuid).o)
	@echo "$(BUILD_OBJS)/$(notdir $*.$(obj_uuid).o)" >> $(BUILD_OBJS)/Objects.list

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
	@$(MAKE) -C $(TOOLDIR) HOST_CC=$(HOST_CC)
	@$(VERSION) $(RC_ProjectName) > $(BUILD_GEN)/$(RC_ProjectName)_version.c
	$(_CC) $(CFLAGS) -c $(BUILD_GEN)/$(RC_ProjectName)_version.c -o $(BUILD_ROOT)/$(RC_ProjectName)_version.o
	@$(IMAGE3MAKER) -t jsdt -f $(BSP_DIR)/$(PLAT_DT_FILE) -o $(BUILD_ROOT)/dtre.img3
	@cd $(BUILD_ROOT) && $(LD) -r -b binary -o dtre.o dtre.img3

$(TARGET): $(OBJS)
	@bash -e $(ARCHIVE) $(AR) $(BUILD_OBJS)/Objects.list $(BUILD_ROOT)/$(TARGET).a
	$(_LD) -T $(SRCROOT)/arch/arm/init/xboot.ld $(BUILD_ROOT)/$(TARGET).a \
		$(BUILD_ROOT)/dtre.o $(BUILD_ROOT)/$(RC_ProjectName)_version.o -o $(BUILD_ROOT)/$(TARGET).elf $(LDFLAGS)
	$(_OBJDMP) -D $(BUILD_ROOT)/$(TARGET).elf > $(BUILD_ROOT)/$(TARGET).list
	$(_OBJCPY) $(BUILD_ROOT)/$(TARGET).elf -g -S -O binary $(SRCROOT)/$(TARGET).bin
	@echo "$(TARGET).bin is ready."

install: all

clean:
	@rm -rf $(BUILD_DIR) $(SRCROOT)/$(TARGET).* $(SRCROOT)/xboot.*.bin
	@$(MAKE) -C $(TOOLDIR) clean
	@echo "Source tree is now clean."


help:
	@echo "When building xBoot, you must set PLATFORM to to a desired platform target.\n"
	@echo -n "Platforms supported by xBoot: "
	@for i in `ls $(SRCROOT)/bsp`; do	\
		echo -n "$$i ";			\
	done;
	@echo "\n\nIf you wish to see commands being called while building, you can also set VERBOSE_BUILD to 1\n"
