#ifndef MEMORY_H
#define MEMORY_H

#include "util.h"

void memset(void* ptr, u8 c, u32 len);
void* memcpy(void* dst, void* src, u32 len);

#endif
