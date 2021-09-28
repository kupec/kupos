KUPOS_IMG = build/kupos.img

.PHONY: all
all: build $(KUPOS_IMG)

build:
	mkdir -p build

$(KUPOS_IMG): boot/disk.img boot/boot.img boot/code_opts.inc
	bash scripts/build_image.sh $^ $@

boot/disk.img: boot/disk_opts.inc kernel/kernel.bin
	bash scripts/generate_disk.sh $< $@

boot/boot.img: boot/boot.o
	objcopy --dump-section .text=$@ $<

boot/boot.o: boot/boot.s
	gcc -m16 -nostdlib -c -o $@ $<

kernel/kernel.bin: kernel/kernel.o
	objcopy --dump-section .text=$@ $<

kernel/kernel.o: kernel/kernel.s
	gcc -m16 -nostdlib -c -o $@ $<

.PHONY: run
run: all
	qemu-system-i386 build/boot.img

.PHONY: disasm
disasm: boot/boot.img
	objdump -D -b binary -mi386 -Maddr16 -Mdata16 $<

.PHONY: clean
clean:
	rm -rf build boot/*.o boot/*.img

