//
// Created by matous on 4/19/20.
//

#include "plane/MainMotorTimer.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>


void MainMotorTimer::init() {
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
}

void MainMotorTimer::setSpeed(uint8_t speed) {
    if (speed >= 20) {
        timer_set_oc_value(TIM3, TIM_OC3, speed * 20000 / 255);
    } else {
        timer_set_oc_value(TIM3, TIM_OC3, 0);
    }
}
