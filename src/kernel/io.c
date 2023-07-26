#include "util.h"

#define PORT 0x3f8          // COM1
int init_serial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(PORT + 0) != 0xAE) {
      return 1;
   }
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(PORT + 4, 0x0F);
   return 0;
}

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(PORT,a);
}



// io port read and write
void outb(u16 port, u8 val){
    // send a byte in val to the port specified
    __asm__ volatile("outb %0, %1":: "a" (val), "Nd" (port): "memory");
}

u8 inb(u16 port){
    // read a byte from the port specified and return
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret): "Nd" (port): "memory");

    return ret;
}

#define UNUSED_PORT 0x80
void io_wait(){
    outb(UNUSED_PORT, 0);
    return;
}
