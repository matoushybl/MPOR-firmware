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
    SPI1_CR1 = SPI_CR1_MSTR | SPI_CR1_MSBFIRST | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA | (SPI_CR1_BR_FPCLK_DIV_64 << 3);
    SPI1_CR2 = SPI_CR2_DS_8BIT | SPI_CR2_FRXTH;

    SPI1_CR1 |= SPI_CR1_SPE;

//    uint8_t read = readSingleByteRegister(static_cast<uint8_t>(Register::RFSetup));
}

uint8_t NRF24::readSingleByteRegister(uint8_t reg) {
    gpio_clear(GPIOA, GPIO4);
    status = spi_xfer8((uint8_t)Command::ReadReg | (reg & 0x1f));
    auto res = spi_xfer8(0);
    gpio_set(GPIOA, GPIO4);
    return res;
}

uint8_t NRF24::spi_xfer8(uint8_t data) {
    SPI1_DR8 = data;
    while ((SPI1_SR & SPI_SR_RXNE) == 0);
    return SPI1_DR8;
}

Status NRF24::getStatus() {
    Status result;
    result.raw = status;
    return result;
}
