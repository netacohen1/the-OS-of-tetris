#ifndef PIC_H
#define PIC_H
#include "util.h"

void pic_init(int offset1, int offset2);
void PIC_sendEOI(u8 irq_line);
void PIC_mask(u8 irq_line);
void PIC_clear_mask(u8 irq_line);
u16 PIC_get_isr();
u16 PIC_get_irr();
#endif
