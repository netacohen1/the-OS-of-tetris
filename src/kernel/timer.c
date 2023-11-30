#include "timer.h"
#include "util.h"
#include "irq.h"

static struct {
    u32 divisor;
    u32 frequency;
    u64 ticks;
} state;

/* u64 timer = 0; */
void timerFunction(Registers_t *regs){
    state.ticks++;
}

void wait(u32 miliseconds){
    u64 start = state.ticks;
    while (state.ticks - start < miliseconds){

    }
}

void PIT_setCount(){
    // set the frequency of the pit to 1/TIMER_FREQ
    state.divisor = TIMER_FREQ; 
    state.frequency = PIT_INTERNAL_FREQUENCY / state.divisor;      // get the desired freq

    CLI();
    outb(PIT_COMMAND, PIT_SET);                     
    outb(PIT_CHANNEL0, state.frequency & 0xff);                  // low byte
    outb(PIT_CHANNEL0, ((state.frequency & 0xff00) >> 8));       // high byte
    STI();

    return;
}

u64 get_timer(){
    return state.ticks;
}

void timer_install(){
    PIT_setCount();
    IRQ_RegisterHandler(0, timerFunction);
}
