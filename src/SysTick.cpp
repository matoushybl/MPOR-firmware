//
// Created by Matous Hybl on 11/01/2020.
//

#include "SysTick.h"
#include <libopencm3/cm3/systick.h>

extern "C" {
    void sys_tick_handler();
}

volatile uint64_t SysTick::systick_millis = 0;

void SysTick::init() {
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    STK_CVR = 0;

    systick_set_reload(48000);
    systick_counter_enable();
    systick_interrupt_enable();
}

void SysTick::delay(uint32_t milliseconds) {
    uint32_t target = systick_millis + milliseconds;
    while (target > systick_millis);
}

uint64_t SysTick::millis() {
    return systick_millis;
}

void sys_tick_handler() {
    SysTick::systick_millis++;
}