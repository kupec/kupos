KUPOS_IMG = build/kupos.img

.PHONY: all
all: build $(KUPOS_IMG)

build:
	mkdir -p build

$(KUPOS_IMG): boot/disk.img boot/mbr.bin boot/boot.bin boot/code_opts.inc
	bash scripts/build_image.sh $^ $@

boot/disk.img: boot/disk_opts.inc kernel/kernel.bin
	bash scripts/generate_disk.sh $< $@

%.bin: %.o
	ld -m elf_i386 -Ttext 0 --entry 0 --oformat binary -o $@ $<

boot/mbr.o: boot/mbr.s boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

boot/boot.o: boot/boot.s boot/code_opts.inc boot/boot.inc
	gcc -m16 -nostdlib -c -o $@ $<

kernel/kernel.o: kernel/kernel.s
	gcc -m16 -nostdlib -c -o $@ $<

.PHONY: clean
clean:
	rm -rf build */*.o */*.bin */*.img

