HOST_CC ?= gcc
CFLAGS := -Wno-multichar

TOOL := image3maker
OBJS := image3maker.o

%.o: %.c
	$(HOST_CC) -c $< $(CFLAGS) -o $@

$(TOOL): $(OBJS)
	$(HOST_CC) $(OBJS) -o $@

all: $(TOOL)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TOOL)
