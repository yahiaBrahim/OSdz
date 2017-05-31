void outportb(unsigned short port, unsigned char data)
{

    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));

}

unsigned char inportb(unsigned short  port)
{
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
