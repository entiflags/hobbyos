all: hobbyos.iso

hobbyos.bin: Boot/Bootloader.o Kernel/Kernel.o Kernel/VGA.o Kernel/GDT.o Kernel/GDT_asm.o Kernel/IDT.o Kernel/IDT_asm.o Kernel/ISR.o Kernel/IRQ_cpp.o Kernel/IRQ_asm.o Kernel/Utils.o Kernel/Keyboard.o Kernel/MemoryManagement/PhysicalMemoryManagement.o linker.ld
	g++ -m32 -T linker.ld -o build/hobbyos.bin -ffreestanding -O2 -nostdlib -no-pie Boot/Bootloader.o Kernel/Kernel.o Kernel/VGA.o Kernel/GDT.o Kernel/GDT_asm.o Kernel/IDT.o Kernel/IDT_asm.o Kernel/ISR.o Kernel/IRQ_cpp.o Kernel/IRQ_asm.o Kernel/Utils.o Kernel/Keyboard.o Kernel/MemoryManagement/PhysicalMemoryManagement.o -lgcc

Boot/Bootloader.o: Boot/Bootloader.asm
	nasm -f elf32 Boot/Bootloader.asm -o Boot/Bootloader.o

Kernel/Kernel.o: Kernel/Kernel.cpp Include/VGA.h Include/IO.h Include/GDT.h Include/IDT.h Include/IRQ.h Include/Keyboard.h Include/PhysicalMemoryManagement.h
	g++ -m32 -c Kernel/Kernel.cpp -o Kernel/Kernel.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/VGA.o: Kernel/VGA.cpp Include/VGA.h Include/IO.h
	g++ -m32 -c Kernel/VGA.cpp -o Kernel/VGA.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/GDT.o: Kernel/GDT.cpp Kernel/GDT.asm Include/GDT.h
	g++ -m32 -c Kernel/GDT.cpp -o Kernel/GDT.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude
	nasm -f elf32 Kernel/GDT.asm -o Kernel/GDT_asm.o

Kernel/IDT.o: Kernel/IDT.cpp Kernel/IDT.asm Include/IDT.h Include/IRQ.h Include/Utils.h
	g++ -m32 -c Kernel/IDT.cpp -o Kernel/IDT.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude
	nasm -f elf32 Kernel/IDT.asm -o Kernel/IDT_asm.o

Kernel/ISR.o: Kernel/ISR.asm Include/IDT.h
	nasm -f elf32 Kernel/ISR.asm -o Kernel/ISR.o

Kernel/IRQ_cpp.o: Kernel/IRQ.cpp Include/IRQ.h Include/VGA.h Include/IO.h
	g++ -m32 -c Kernel/IRQ.cpp -o Kernel/IRQ_cpp.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/IRQ_asm.o: Kernel/IRQ.asm Include/IRQ.h
	nasm -f elf32 Kernel/IRQ.asm -o Kernel/IRQ_asm.o

Kernel/Utils.o: Kernel/Utils.cpp Include/Utils.h
	g++ -m32 -c Kernel/Utils.cpp -o Kernel/Utils.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/Keyboard.o: Kernel/Keyboard.cpp Include/Keyboard.h Include/VGA.h Include/IO.h
	g++ -m32 -c Kernel/Keyboard.cpp -o Kernel/Keyboard.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

Kernel/MemoryManagement/PhysicalMemoryManagement.o: Kernel/MemoryManagement/PhysicalMemoryManagement.cpp Include/PhysicalMemoryManagement.h Include/Utils.h
	g++ -m32 -c Kernel/MemoryManagement/PhysicalMemoryManagement.cpp -o Kernel/MemoryManagement/PhysicalMemoryManagement.o -std=c++17 -ffreestanding -O2 -Wall -Wextra -IInclude

hobbyos.iso: hobbyos.bin Boot/grub.cfg
	mkdir -p build/isodir/boot/grub
	cp build/hobbyos.bin build/isodir/boot/hobbyos.bin
	cp Boot/grub.cfg build/isodir/boot/grub/grub.cfg
	grub-mkrescue -o hobbyos.iso build/isodir

clean:
	rm -rf Boot/*.o Kernel/*.o build/* *.iso

.PHONY: all clean
