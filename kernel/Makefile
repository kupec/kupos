include kernel.inc

CC = gcc
CFLAGS = -m32 -nostdlib -fno-pic -fno-asynchronous-unwind-tables

SOURCES := $(shell find . -type f -name '*.c')
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS := $(patsubst %.c,%.d,$(SOURCES))

.PHONY: all
all: kernel.bin
	
kernel.bin: mode16.bin main.bin
	cat $^ > $@

mode16.bin: mode16.o
	ld -m elf_i386 -Ttext 0 --entry 0 --defsym KERNEL_ENTRYPOINT=$(KERNEL_ENTRYPOINT) --oformat binary -o $@ $<

main.bin: build.ld prot.o $(OBJECTS)
	ld -m elf_i386 -shared -fno-pie -o $@ -T $^

mode16.o: mode16.s ../boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

prot.o: prot.s
	gcc -m32 -nostdlib -c -o $@ $<

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECT) $(DEPENDS) *.bin
