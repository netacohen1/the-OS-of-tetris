#include "util.h"
#include "memory.h"

u32* memset(u32* ptr, u8 c, u32 len){
    u32* startPtr = ptr;
    while(len--){
        *ptr++ = (u8)c;
    }
    return startPtr;
}
