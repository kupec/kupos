all: build_dir build/boot.img

build_dir:
	mkdir -p build

build/boot.img: boot/boot.o
	gobjcopy --dump-section .text=$@ $<

boot/boot.o: boot/boot.S
	as -arch i386 -o $@ $<

run: all
	qemu-system-i386 build/boot.img

disasm: build/boot.img
	gobjdump -D -b binary -mi386 -Maddr16 -Mdata16 $<

clean:
	rm -rf build boot/*.o

