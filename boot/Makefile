.PHONY: all
all: boot.bin mbr.bin
	

%.bin: %.o
	ld -m elf_i386 -Ttext 0 --entry 0 --oformat binary -o $@ $<

%.o: %.s boot.inc code_opts.inc
	gcc -m16 -nostdlib -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.bin

