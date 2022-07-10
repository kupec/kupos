#include "timer.h"
#include "../../asm/int.h"

extern volatile uint timer_ticks;

void sleep(uint ms) {
    timer_ticks = 18.2 * ms / 1000;
    while (timer_ticks > 0) {
        hlt();
    }
}
