/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>

#include "../libs/RF24/RF24.h"

#include <plane/Logger.h>
#include <plane/SysTick.h>
#include <plane/ServoTimer.h>
#include <plane/MainMotorTimer.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

struct __attribute__((__packed__)) ControlData {
    uint8_t mainMotorPower = 0;
    int8_t rudderPosition = 0;
    int8_t elevatorPosition = 0;
    int8_t leftAileronPosition = 0;
    int8_t rightAileronPosition = 0;
};

int main() {
    rcc_clock_setup_in_hsi_out_48mhz();
//    nvic_enable_irq(NVIC_TIM3_IRQ);

//    Logger::init();
    SysTick::init();
    ServoTimer::init();
    MainMotorTimer::init();

    ServoTimer::setPosition(Servo::Elevator, 0);
    ServoTimer::setPosition(Servo::Rudder, 0);
    ServoTimer::setPosition(Servo::LeftAileron, 0);
    ServoTimer::setPosition(Servo::RightAileron, 0);

    MainMotorTimer::setSpeed(0);

    RF24 radio;
    SysTick::delay(1000);

    uint8_t addresses[][6] = {"1Node","2Node"};

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);

    radio.startListening();

    while (true) {
        ControlData controlData;

        if(radio.available()){
            while (radio.available()) {
                radio.read(&controlData, sizeof(ControlData));

                MainMotorTimer::setSpeed(controlData.mainMotorPower);
                ServoTimer::setPosition(Servo::Elevator, controlData.elevatorPosition);
                ServoTimer::setPosition(Servo::Rudder, controlData.rudderPosition);
                ServoTimer::setPosition(Servo::LeftAileron, controlData.leftAileronPosition);
                ServoTimer::setPosition(Servo::RightAileron, controlData.rightAileronPosition);
            }
        }
    }
    return 0;
}

#pragma clang diagnostic pop