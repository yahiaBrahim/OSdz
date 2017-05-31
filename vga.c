#include <io.h>
#include <types.h>

char cursor_x = 0; char cursor_y = 0;
u16int *video_memory = (u16int *)0xB8000;

void clear_screen()
{
    unsigned char* vidmem = (unsigned char*) 0xb8000;
    unsigned int i=0;
    while(i<(80*25*2))
    {
        vidmem[i] = ' ';
        ++i;
        vidmem[i] = 0x07;
        ++i;
    };
}
void scroll()
{

   // Get a space character with the default colour attributes.
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= 25)
   {
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           video_memory[i] = video_memory[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++)
       {
           video_memory[i] = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = 24;
   }
} 

void update_cursor(int x, int y)
{
    unsigned short    position=(y*80) + x; 
    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char)(position));//1111 1111 1100 0000
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char)((position>>8)));//1111 1111 1100 0000 --> 0000 0000 1111 1111
}

void write_ch(char c)
{
   // The background colour is black (0), the foreground is white (15).
   u8int backColour = 0;
   u8int foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   u16int attribute = attributeByte << 8;
   u16int *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1);
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = video_memory + (cursor_y*80 + cursor_x);
       *location = c | attribute;
        cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y ++;
   }

   // Scroll the screen if needed.
   scroll();
   // Move the hardware cursor.
   update_cursor(cursor_x, cursor_y);
} 

void write_st(char *s)
{

    uint32_t i;

    for (i = 0; s[i] != '\0'; i++)
        write_ch(s[i]);

}

void write_dec(uint32_t n)
{
  if (n == 0)
  {
    write_ch('0');
    return;
  }

  uint32_t acc = n;
  char c[32];
  int i = 0;
  while (acc > 0)
  {
      c[i] = '0' + acc%10;
      acc /= 10;
      i++;
  }
  c[i] = 0;

  char c2[32];
  c2[i--] = 0;
  int j = 0;
  while(i >= 0)
    c2[i--] = c[j++];
  write_st(c2);
}