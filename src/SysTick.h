//
// Created by Matous Hybl on 11/01/2020.
//

#ifndef PWM_SYSTICK_H
#define PWM_SYSTICK_H


#include <cstdint>

class SysTick {
public:
    static void init();
    static uint64_t millis();
    static void delay(uint32_t milliseconds);

    static void delayMicroseconds(uint64_t us);

    static volatile uint64_t systick_millis;
};


#endif //PWM_SYSTICK_H
