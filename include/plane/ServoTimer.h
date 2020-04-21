//
// Created by Matous Hybl on 11/01/2020.
//

#ifndef PWM_SERVOTIMER_H
#define PWM_SERVOTIMER_H

#include <stdint-gcc.h>

enum class Servo {
    Rudder,
    Elevator,
    LeftAileron,
    RightAileron
};

class ServoTimer {
public:
    static void init();

    static void setPosition(Servo servo, int8_t position);
};


#endif //PWM_SERVOTIMER_H
