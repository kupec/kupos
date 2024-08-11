include kernel.inc

CC = clang
CFLAGS = -Werror -m32 -nostdlib -fno-pic -fno-asynchronous-unwind-tables -ffreestanding

SOURCES := $(shell find . -type f -name '*.c')
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS := $(patsubst %.c,%.d,$(SOURCES))

.PHONY: all
all: kernel.bin
	
kernel.bin: mode16.bin main.bin
	cat $^ > $@

mode16.bin: mode16.o
	ld -m elf_i386 -Ttext 0 --entry 0 --defsym KERNEL_ENTRYPOINT=$(KERNEL_ENTRYPOINT) --oformat binary -o $@ $<

main.bin: main.elf
	objcopy $< -O binary $@

main.elf: build.ld prot.o int.o $(OBJECTS)
	ld -m elf_i386 -o $@ -T $^

mode16.o: mode16.s ../boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

prot.o: prot.s
	gcc -m32 -nostdlib -c -o $@ $<

int.o: int.s
	gcc -m32 -nostdlib -c -o $@ $<

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -g -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(DEPENDS) *.bin *.elf *.o
