include kernel.inc

CC = gcc
CFLAGS = -m32 -nostdlib -fno-pic -fno-asynchronous-unwind-tables
objects = main.o console.o memory/memory.o asm/int.o

.PHONY: all
all: kernel.bin
	
kernel.bin: mode16.bin main.bin
	cat $^ > $@

mode16.bin: mode16.o
	ld -m elf_i386 -Ttext 0 --entry 0 --defsym KERNEL_ENTRYPOINT=$(KERNEL_ENTRYPOINT) --oformat binary -o $@ $<

main.bin: build.ld prot.o $(objects)
	ld -m elf_i386 -shared -fno-pie -o $@ -T $^

mode16.o: mode16.s ../boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

prot.o: prot.s
	gcc -m32 -nostdlib -c -o $@ $<

main.o: main.c console.h string.h memory/memory.h
console.o: console.c math.h console.h string.h asm/io.h types/types.h
memory/memory.o: memory/memory.c memory/memory.h types/types.h asm/int.h
asm/int.o: asm/int.c asm/int.h

.PHONY: clean
clean:
	rm -rf **/*.o *.o *.bin
