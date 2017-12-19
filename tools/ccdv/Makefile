HOST_CC ?= gcc
CFLAGS :=

TOOL := ccdv
OBJS := ccdv.o

%.o: %.c
	$(HOST_CC) -c $< $(CFLAGS) -o $@

$(TOOL): $(OBJS)
	$(HOST_CC) $(OBJS) -o $@

all: $(TOOL)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TOOL)
