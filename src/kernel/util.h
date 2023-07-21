#ifndef UTIL_H
#define UTIL_H

// fixed width integer types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef u32 size_t;
typedef u32 uintptr_t;
typedef float f32;
typedef double f64;

// define bool
typedef u8 bool;
#define true (1)
#define false (0)

// CLI and STI
#define CLI() __asm__ volatile("cli")
#define STI() __asm__ volatile("sti")

// set and unset a flag
#define SETFLAG(x, flag) x |= flag
#define UNSETFLAG(x, flag) x &= ~flag

// define PACKED
#define PACKED __attribute__((packed))

// io port read and write
static inline void outb(u16 port, u8 val){
    // send a byte in val to the port specified
    __asm__ volatile("outb %0, %1":: "a" (val), "Nd" (port): "memory");
}

static inline u8 inb(u16 port){
    // read a byte from the port specified and return
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a" (ret): "Nd" (port): "memory");

    return ret;
}

#define UNUSED_PORT 0x80
static inline void io_wait(){
    outb(UNUSED_PORT, 0);
    return;
}

#endif
