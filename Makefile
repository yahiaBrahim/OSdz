
CC=gcc #i386-osdz-elf-gcc
LD=ld #i386-osdz-elf-ld
AS=nasm


CCFLAGS=-c -I ../include -Wall -ffreestanding -nostdlib \
	-nostartfiles -nodefaultlibs -fno-builtin -m32 -g 

ASFLAGS=-f elf

OBJ= loader.o main.o vga.o io.o string.o gdt_s.o gdt.o idt_s.o idt.o pit.o



all: $(OBJ)
	$(LD) -Tlinker.ld -o dzkernel $(OBJ)

%.o: %.s
	@$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c 
	@$(CC) $(CCFLAGS) -o $@ $<

run: all cd
	qemu-system-i386 -cdrom OSdz.iso

cd: all
	@cp -f dzkernel dz/boot/
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot \
	-boot-load-size 4 -boot-info-table -o OSdz.iso dz



clean:
	@rm -f *.o dzkernel


