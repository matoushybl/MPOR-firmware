//
// Created by Matous Hybl on 11/01/2020.
//

#include "ServoTimer.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

void ServoTimer::init() {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_af(GPIOA, GPIO_AF2, GPIO0 | GPIO1 | GPIO2 | GPIO3);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_reset_pulse(RST_TIM2);

    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM2, 48);
    timer_set_period(TIM2, 20000);
    timer_enable_break_main_output(TIM2);

    timer_enable_oc_output(TIM2, TIM_OC1);
    timer_enable_oc_output(TIM2, TIM_OC2);
    timer_enable_oc_output(TIM2, TIM_OC3);
    timer_enable_oc_output(TIM2, TIM_OC4);

    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM2, TIM_OC4, TIM_OCM_PWM1);

    timer_set_oc_value(TIM2, TIM_OC1, 500);
    timer_set_oc_value(TIM2, TIM_OC2, 800);
    timer_set_oc_value(TIM2, TIM_OC3, 500);
    timer_set_oc_value(TIM2, TIM_OC4, 500);

    timer_enable_counter(TIM2);
}