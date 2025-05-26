all: hobbyos.iso

hobbyos.bin: Boot/Bootloader.o Kernel/Kernel.o Kernel/VGA.o Kernel/GDT.o Kernel/GDT_asm.o linker.ld
	g++ -m32 -T linker.ld -o build/hobbyos.bin -ffreestanding -O2 -nostdlib Boot/Bootloader.o Kernel/Kernel.o Kernel/VGA.o Kernel/GDT.o Kernel/GDT_asm.o -lgcc

Boot/Bootloader.o: Boot/Bootloader.asm
	nasm -f elf32 Boot/Bootloader.asm -o Boot/Bootloader.o

Kernel/Kernel.o: Kernel/Kernel.cpp Include/VGA.h Include/IO.h Include/GDT.h
	g++ -m32 -c Kernel/Kernel.cpp -o Kernel/Kernel.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/VGA.o: Kernel/VGA.cpp Include/VGA.h Include/IO.h
	g++ -m32 -c Kernel/VGA.cpp -o Kernel/VGA.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/GDT.o: Kernel/GDT.cpp Kernel/GDT.asm Include/GDT.h
	g++ -m32 -c Kernel/GDT.cpp -o Kernel/GDT.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude
	nasm -f elf32 Kernel/GDT.asm -o Kernel/GDT_asm.o

hobbyos.iso: hobbyos.bin Boot/grub.cfg
	mkdir -p build/isodir/boot/grub
	cp build/hobbyos.bin build/isodir/boot/hobbyos.bin
	cp Boot/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o hobbyos.iso build/isodir

clean:
	rm -rf Boot/*.o Kernel/*.o build/* *.iso

.PHONY: all clean
