#include <vga.h>
#include <types.h>
#include <idt.h>
#include <io.h>


u32int tick = 0;

static void timer_callback(registers_t regs)
{
   tick++;
   write_st("Tick: ");
   write_dec(tick);
   write_st("\n");
}

void init_timer(u32int frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, (interrupt_handler_t)&timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   u32int divisor = 1193180 / frequency;

   // Send the command byte.
   outportb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   u8int l = (u8int)(divisor & 0xFF);
   u8int h = (u8int)( (divisor>>8) & 0xFF );

   // Send the frequency divisor.
   outportb(0x40, l);
   outportb(0x40, h);
} 