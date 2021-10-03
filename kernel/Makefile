include kernel.inc

.PHONY: all
all: kernel.bin
	
kernel.bin: mode16.bin main.bin
	cat $^ > $@

mode16.bin: mode16.o
	ld -m elf_i386 -Ttext 0 --entry 0 --defsym KERNEL_ENTRYPOINT=$(KERNEL_ENTRYPOINT) --oformat binary -o $@ $<

main.bin: prot.o main.o
	ld -m elf_i386 -Ttext $(KERNEL_ENTRYPOINT) --entry 0 --oformat binary -shared -fno-pie -o $@ $^

mode16.o: mode16.s ../boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

prot.o: prot.s
	gcc -m32 -nostdlib -c -o $@ $<

main.o: main.c
	gcc -m32 -nostdlib -fno-pic -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.bin