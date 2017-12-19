HOST_CC ?= gcc

TOOLS := \
	ccdv \
	image3maker

define do_make
	@for dir in $1; do \
		$(MAKE) -C $$dir HOST_CC=$(HOST_CC) $2; \
	done
endef

all:
	$(call do_make, $(TOOLS), all)

.PHONY: clean
clean:
	$(call do_make, $(TOOLS), clean)
