all:hobbyos.iso

hobbyos.bin:Bootloader.o Kernel.o linker.ld
	g++ -m32 -T linker.ld -o hobbyos.bin -ffreestanding -O2 -nostdlib Bootloader.o Kernel.o -lgcc

Bootloader.o:Bootloader.asm
	nasm -f elf32 Bootloader.asm -o Bootloader.o

Kernel.o:Kernel.cpp
	g++ -m32 -c Kernel.cpp -o Kernel.o -std=c++17 -ffreestanding -O2 -Wall -Wextra

hobbyos.iso:hobbyos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp hobbyos.bin isodir/boot/hobbyos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o hobbyos.iso isodir

clean:
	rm -rf *.o *.bin *.iso isodir

.PHONY:all clean
