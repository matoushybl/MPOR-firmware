//
// Created by matous on 4/19/20.
//

#ifndef PWM_MAINMOTORTIMER_H
#define PWM_MAINMOTORTIMER_H


#include <cstdint>

class MainMotorTimer {
public:
    static void init();

    static void setSpeed(uint8_t speed);
};


#endif //PWM_MAINMOTORTIMER_H
