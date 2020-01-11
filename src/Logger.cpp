//
// Created by Matous Hybl on 11/01/2020.
//

#include "Logger.h"

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include "printf.h"

extern "C" {
void _putchar(char character);
void dma1_channel2_3_dma2_channel1_2_isr();
}


uint8_t Logger::printfBuffer[50] = {0};
uint8_t Logger::printfBufferIndex = 0;

uint8_t Logger::dmaBuffer[50] = {0};
uint8_t Logger::dmaTransferSize = 0;
bool Logger::dmaTransferDone = true;

void Logger::init() {
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_af(GPIOA, GPIO_AF1, GPIO9);

    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable(USART1);

    // DMA
    rcc_periph_clock_enable(RCC_DMA1);
    rcc_periph_clock_enable(RCC_DMA);
    nvic_enable_irq(NVIC_DMA1_CHANNEL2_3_DMA2_CHANNEL1_2_IRQ);
    nvic_set_priority(NVIC_DMA1_CHANNEL2_3_DMA2_CHANNEL1_2_IRQ, 0);
}

void dma1_channel2_3_dma2_channel1_2_isr() {
    if ((DMA1_ISR & DMA_ISR_TCIF2) != 0) {
        DMA1_IFCR |= DMA_IFCR_CTCIF2;

        Logger::dmaTransferDone = true;
    }

    dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);

    usart_disable_tx_dma(USART1);

    dma_disable_channel(DMA1, DMA_CHANNEL2);
}

void _putchar(char character) {
    if (character == '\n') {
        Logger::printfBuffer[Logger::printfBufferIndex] = '\r';
        Logger::printfBuffer[Logger::printfBufferIndex] = '\n';
        Logger::printfBufferIndex += 2;

        if (Logger::dmaTransferDone) {
            for (int i = 0; i < Logger::printfBufferIndex; i++) {
                Logger::dmaBuffer[i] = Logger::printfBuffer[i];
            }
            Logger::dmaTransferSize = Logger::printfBufferIndex - 1;
            dma_channel_reset(DMA1, DMA_CHANNEL2);

            dma_set_peripheral_address(DMA1, DMA_CHANNEL2, (uint32_t) &USART1_TDR);
            dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t) Logger::dmaBuffer);
            dma_set_number_of_data(DMA1, DMA_CHANNEL2, Logger::dmaTransferSize);
            dma_set_read_from_memory(DMA1, DMA_CHANNEL2);
            dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL2);
            dma_set_peripheral_size(DMA1, DMA_CHANNEL2, DMA_CCR_PSIZE_8BIT);
            dma_set_memory_size(DMA1, DMA_CHANNEL2, DMA_CCR_MSIZE_8BIT);
            dma_set_priority(DMA1, DMA_CHANNEL2, DMA_CCR_PL_VERY_HIGH);

            dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL2);

            dma_enable_channel(DMA1, DMA_CHANNEL2);

            usart_enable_tx_dma(USART1);
            Logger::dmaTransferDone = false;
            Logger::printfBufferIndex = 0;
        }
    } else {
        Logger::printfBuffer[Logger::printfBufferIndex] = character;
        Logger::printfBufferIndex++;
    }
}

