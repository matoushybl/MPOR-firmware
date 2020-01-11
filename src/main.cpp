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

    radio.reset();
    radio.setFrequencyMHz(2500);
    radio.setPayloadWidth(0, 5);
    radio.setPayloadWidth(1, 5);
    radio.setTransmitAddress((uint8_t *) "prdel");
    radio.setReceiveAddress(1, (uint8_t *) "prdyy");

    while (true) {
//        radio.writeSingleByteRegister(Register::EnableRXAddr, addressEnable.raw);

//        config.raw = radio.readSingleByteRegister(0x00);
//        config.powerUp = 1;
//        radio.writeSingleByteRegister(0x00, config.raw);
//        auto config = radio.readSingleByteRegister(Register::RXPayloadWidthPipe1);
        auto fifo = radio.readFIFOStatus();
        printf("status: %x reg: %x\n", fifo.raw, 0);
        if ((fifo.raw & 0b00010000) > 0) {
            radio.writeTxPayload((uint8_t *) "prdel", 5);
        }
        SysTick::delay(10);
    }
    return 0;
}