#ifndef TIMER_H
#define TIMER_H
#include "util.h"


#define TIMER_FREQ              100
#define PIT_INTERNAL_FREQUENCY  1193182

#define PIT_CHANNEL0    0x40
#define PIT_CHANNEL1    0x41
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43

#define PIT_SET         0x36

void timer_install();
u64 get_timer();

#endif
