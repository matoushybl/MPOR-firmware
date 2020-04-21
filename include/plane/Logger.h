//
// Created by Matous Hybl on 11/01/2020.
//

#ifndef PWM_LOGGER_H
#define PWM_LOGGER_H

#include <cstdint>

class Logger {
public:
    static uint8_t printfBuffer[50];
    static uint8_t printfBufferIndex;

    static uint8_t dmaBuffer[50];
    static uint8_t dmaTransferSize;
    static bool dmaTransferDone;

    static void init();
};


#endif //PWM_LOGGER_H
