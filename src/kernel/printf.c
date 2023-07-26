#include "util.h"

// define qemu logs
void QemuPrintStr(i8* s){
    while(*s != 0){
        write_serial(*s++); // changed
    }
}

// convert binary number to ascii
const i8 g_chars[] = "0123456789abcdef";
void printf_unsigned(u32 num, u32 base){
    i8 string[32];
    i32 pos = 0;

    do {
        u32 rem = num % base;
        num /= base;
        string[pos++] = g_chars[rem];
    } while (num > 0);

    while (--pos >= 0){
        write_serial(string[pos]); // changed
    }
}

void printf_signed(i32 num, u32 base){
    if (num < 0){
        write_serial('-'); // changed
        printf_unsigned(-num, base);
    }
    else printf_unsigned(num, base);
}

// implement printf
#define STATE_NORMAL 0
#define STATE_FORMAT 1
void QemuPrintf(const i8* str, ...){
    u32* argp = (u32*)&str;
    u8 state = STATE_NORMAL;


    argp += sizeof(str)/sizeof(u32);
    while (*str){
        switch (state){
            case STATE_NORMAL:                      // wait for % or print
                switch (*str){
                    case '%':
                        state = STATE_FORMAT;
                        str++;
                        break;
                    default:
                        write_serial(*str); // changed
                        goto TEST_;
                }
            // fall through
            case STATE_FORMAT:                      // check what char is after %
                switch (*str){
                    case '%':
                        write_serial('%'); // changed
                        break;

                    case 'c':                       // print char
                        write_serial((char)*argp); // changed
                        argp++;
                        break;

                    case 's':                       // print string
                        QemuPrintStr((char*)*argp);
                        argp++;
                        break;

                    case 'd':                       // print signed base 10
                        printf_signed((i32)*argp, 10);
                        break;

                    case 'u':                       // print unsigned base 10
                        printf_unsigned((u32)*argp, 10);
                        break;

                    case 'x':
                    case 'p':                       // print unsigned base 16
                        printf_unsigned((u32)*argp, 16);
                        break;

                    default: break;                 // ignore invalid char
                }
        TEST_:
            state = STATE_NORMAL;
        }
        str++;
    }
    return;
}
