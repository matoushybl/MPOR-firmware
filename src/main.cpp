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

#include "printf.h"
#include "Logger.h"
#include "SysTick.h"
#include "ServoTimer.h"
#include "NRF24.h"

int main() {
    rcc_clock_setup_in_hsi_out_48mhz();

    Logger::init();
    SysTick::init();
    ServoTimer::init();

    NRF24 radio;
    SysTick::delay(1000);

    while (true) {
        auto config = radio.readSingleByteRegister(0x00);
        printf("status: %x reg: %x\n", radio.getStatus().raw, config);
        SysTick::delay(10);
    }
    return 0;
}