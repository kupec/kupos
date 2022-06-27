BINARIES := boot.bin mbr.bin
DEBUGS := $(patsubst %.bin,%.elf,$(BINARIES))

.PHONY: all
all: $(BINARIES) $(DEBUGS)
	

%.bin: %.o
	ld -m elf_i386 -Ttext 0 --entry 0 --oformat binary -o $@ $<

%.elf: %.o
	ld -m elf_i386 -Ttext 0x7c00 --entry 0 -o $@ $<

%.o: %.s boot.inc opts.inc
	gcc -m16 -nostdlib -c -g -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.bin *.elf

