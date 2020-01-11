//
// Created by Matouš Hýbl on 24/12/2019.
//

#include "NRF24.h"
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

NRF24::NRF24() {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3  | GPIO5 | GPIO6 | GPIO7);
    gpio_set_af(GPIOA, GPIO_AF0, GPIO5 | GPIO6 | GPIO7);


    gpio_set(GPIOA, GPIO4);
    SPI1_CR1 &= ~SPI_CR1_SPE;
    SPI1_CR1 = SPI_CR1_MSTR | SPI_CR1_MSBFIRST | SPI_CR1_SSM | SPI_CR1_SSI | (SPI_CR1_BR_FPCLK_DIV_4 << 3);
    SPI1_CR2 = SPI_CR2_DS_8BIT | SPI_CR2_FRXTH;

    SPI1_CR1 |= SPI_CR1_SPE;

//    uint8_t read = readSingleByteRegister(static_cast<uint8_t>(Register::RFSetup));
}

Status NRF24::readStatus() {
    Status result;
    result.raw = readSingleByteRegister(Register::Status);
    return result;
}

uint8_t NRF24::spi_xfer8(uint8_t data) {
    SPI1_DR8 = data;
    while ((SPI1_SR & SPI_SR_RXNE) == 0);
    return SPI1_DR8;
}

uint8_t NRF24::readSingleByteRegister(Register reg) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::ReadReg | ((uint8_t)reg & 0x1f));
    auto res = spi_xfer8(static_cast<uint8_t>(Command::NOP));
    gpio_set(GPIOA, GPIO4);
    return res;
}

uint8_t NRF24::writeSingleByteRegister(Register reg, uint8_t value) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::WriteReg | ((uint8_t)reg & 0x1f));
    spi_xfer8(value);
    gpio_set(GPIOA, GPIO4);
    return status;
}

uint8_t NRF24::readMultibyteRegister(Register reg, uint8_t *output, uint8_t length) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::ReadReg | ((uint8_t)reg & 0x1f));
    for (int i = 0; i < length; ++i) {
        output[i] = spi_xfer8(static_cast<uint8_t>(Command::NOP));
    }
    gpio_set(GPIOA, GPIO4);
    return status;
}

uint8_t NRF24::writeMultibyteRegister(Register reg, uint8_t *input, uint8_t length) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::WriteReg | ((uint8_t)reg & 0x1f));
    for (int i = 0; i < length; ++i) {
        spi_xfer8(input[i]);
    }
    gpio_set(GPIOA, GPIO4);
    return status;
}

void NRF24::reset() {
    writeSingleByteRegister(Register::Config, 0x08);
    writeSingleByteRegister(Register::EnableAutoAck, 0x3f);
    writeSingleByteRegister(Register::EnableRXAddr, 0x03);
    writeSingleByteRegister(Register::SetupAddressWidth, 0x03);
    writeSingleByteRegister(Register::SetupRetransmission, 0x03);
    writeSingleByteRegister(Register::RFChannel, 0x02);
    writeSingleByteRegister(Register::RFSetup, 0x0e);
    writeSingleByteRegister(Register::Status, 0x0e);
    writeSingleByteRegister(Register::ObserveTX, 0x00);
    writeSingleByteRegister(Register::ReceivedPowerDetector, 0x00);
    writeSingleByteRegister(Register::FIFOStatus, 0x09);
    writeSingleByteRegister(Register::DynamicPayloadLength, 0x00);
    writeSingleByteRegister(Register::Feature, 0x00);
}

void NRF24::setFrequencyMHz(uint16_t frequency) {
    writeSingleByteRegister(Register::RFChannel, (frequency - 2400) & 0xff);
}

void NRF24::setTransmitAddress(uint8_t *address) {
    writeMultibyteRegister(Register::TXAddress, address, 5);
    setReceiveAddress(0, address);
}

void NRF24::setReceiveAddress(uint8_t pipe, uint8_t *address) {
    writeMultibyteRegister(static_cast<Register>(0x0A + pipe), address, 5);
}

void NRF24::setPayloadWidth(uint8_t pipe, uint8_t width) {
    writeSingleByteRegister(static_cast<Register>(0x11 + pipe), width & 0x3f);
}

FIFOStatus NRF24::readFIFOStatus() {
    FIFOStatus result {};
    result.raw = readSingleByteRegister(Register::FIFOStatus);
    return result;
}

void NRF24::readRxPayload(uint8_t *output, uint8_t length) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8(static_cast<uint8_t>(Command::ReadRXPayload));
    for (int i = 0; i < length; ++i) {
        output[i] = spi_xfer8(static_cast<uint8_t>(Command::NOP));
    }
    gpio_set(GPIOA, GPIO4);
}

void NRF24::writeTxPayload(uint8_t *output, uint8_t length) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::WriteTXPayload);
    for (int i = 0; i < length; ++i) {
        spi_xfer8(output[i]);
    }
    gpio_set(GPIOA, GPIO4);
}
