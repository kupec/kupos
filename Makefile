all: build_dir build/boot.img

build_dir:
	mkdir -p build

build/boot.img: boot/boot.o
	objcopy --dump-section .text=$@ $<

boot/boot.o: boot/boot.S
	gcc -m16 -nostdlib -c -o $@ $<

run: all
	qemu-system-i386 build/boot.img

disasm: build/boot.img
	objdump -D -b binary -mi386 -Maddr16 -Mdata16 $<

clean:
	rm -rf build boot/*.o

