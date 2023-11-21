#include "util.h"
#include "memory.h"

void memset(void* src, u8 val, size_t len){
    u8* ptr = (u8*)src;
    while(len--){
        *ptr++ = val;
    }
}

void* memcpy(void* dst, void* src, u32 len){
    u8 *d = (u8*)dst;
    const u8 *s = (const u8*)src;
    
    while (len-- > 0){
        *d++ = *s++;
    }
    return dst;
}
