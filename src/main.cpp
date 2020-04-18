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
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "../libs/RF24/RF24.h"

#include "printf.h"
#include "Logger.h"
#include "SysTick.h"
#include "ServoTimer.h"
#include "NRF24.h"

int main() {
    rcc_clock_setup_in_hsi_out_48mhz();
//    nvic_enable_irq(NVIC_TIM3_IRQ);

//    Logger::init();
    SysTick::init();
    ServoTimer::init();

    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);
    gpio_set_af(GPIOB, GPIO_AF1, GPIO0);

    rcc_periph_clock_enable(RCC_TIM3);
    rcc_periph_reset_pulse(RST_TIM3);

    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM3, 48);
    timer_set_period(TIM3, 20000);
    timer_enable_break_main_output(TIM3);

    timer_enable_oc_output(TIM3, TIM_OC3);

    timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);

    timer_set_oc_value(TIM3, TIM_OC3, 0);

    timer_clear_flag(TIM3, TIM_SR_UIF);

    timer_enable_counter(TIM3);

    RF24 radio;
    SysTick::delay(1000);

    uint8_t addresses[][6] = {"1Node","2Node"};

    bool a = radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);

    radio.startListening();

    while (true) {
        unsigned long got_time;

        if( radio.available()){
            // Variable for the received timestamp
            while (radio.available()) {                                   // While there is data ready
                radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
            }

            radio.stopListening();                                        // First, stop listening so we can talk
            radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.
            radio.startListening();
        }
    }
    return 0;
}
