#include <types.h>
#include <multiboot.h>
#include <vga.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>


void my_handler(){

	write_st("\nDivide by zero\n");
	__asm__ __volatile__ ("hlt");
}

void kmain(unsigned long magic, struct multiboot *mbh)
 {


 	clear_screen();
 	update_cursor(0 , 0);



/* 	unsigned char *VIDEO_MEM; 
	VIDEO_MEM = (unsigned char *) 0xB8000;
 	
 	VIDEO_MEM[0]  = 'O';
 	VIDEO_MEM[1]  = (BLACK|WHITE);
 	update_cursor(1 , 0);
 	VIDEO_MEM[2]  = 'S';
 	VIDEO_MEM[3]  = (BLACK|RED);
 	update_cursor(2 , 0);
 	VIDEO_MEM[4]  = 'd';
 	VIDEO_MEM[5]  = (BLACK|GREEN);
 	update_cursor(3 , 0);
 	VIDEO_MEM[6]  = 'z';
 	VIDEO_MEM[7]  = (BLACK|RED);
 	update_cursor(4 , 0);
 */
 //write_st("Hello\n from\tthe Other Side");
 init_gdt();
 init_idt();
 init_timer(50);

 __asm__ __volatile__ ("sti");

 	for(;;);


 }

