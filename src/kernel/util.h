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

// CLI, STI, HLT and BreakPoint
#define CLI() __asm__ volatile("cli")
#define STI() __asm__ volatile("sti")
#define HLT() __asm__ volatile("hlt")
#define BP()  __asm__ volatile("xchgw %bx, %bx"); // bochs magic break point

// set and unset a flag
#define SETFLAG(x, flag) x |= flag
#define UNSETFLAG(x, flag) x &= ~flag

// define PACKED
#define PACKED __attribute__((packed))

// define CDECL
#define CDECL __attribute__((cdecl))


// declare printf --> printf.c
void QemuPrintStr(i8* s);
void QemuPrintf(const i8* str, ...);

// communiation via ports --> io.c
int init_serial();
void write_serial(char a);
void io_wait();
u8 inb(u16 port);
void outb(u16 port, u8 val);

// initialize things --> hal.c
void HAL_init();

#endif
