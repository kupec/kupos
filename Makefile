KUPOS_IMG = build/kupos.img

.PHONY: all
all: build build_recursive $(KUPOS_IMG)

build:
	mkdir -p build

.PHONY: build_recursive
build_recursive:
	make -C boot
	make -C kernel

$(KUPOS_IMG): build/disk.img boot/mbr.bin boot/boot.bin boot/code_opts.inc
	bash scripts/build_image.sh $^ $@

build/disk.img: boot/disk_opts.inc kernel/kernel.bin
	bash scripts/generate_disk.sh $< $@

.PHONY: clean
clean:
	make -C boot clean
	make -C kernel clean
	rm -rf build

