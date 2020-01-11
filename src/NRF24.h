//
// Created by Matouš Hýbl on 24/12/2019.
//

#ifndef UART_LOGGER_NRF24_H
#define UART_LOGGER_NRF24_H

#include <cstdint>

enum class Command: uint32_t {
    ReadReg = 0b00000000, // last five bits contain register address
    WriteReg = 0b00100000, // last five bits contain register address
    ReadRXPayload = 0b01100001,
    WriteTXPayload = 0b10100000,
    FlushTX = 0b11100001,
    FlushRX = 0b11100010,
    ReuseLastTXPayload = 0b11100011,
    ReadRXPayloadWidth = 0b01100000,
    WriteACKPayload = 0b10101000, // last three bits contains pipe number
    WriteTXPayloadNoAck = 0b10110000,
    NOP = 0b11111111
};

enum class Register: uint8_t {
    Config = 0x00,
    EnableAutoAck = 0x01, // shockburst
    EnableRXAddr = 0x02,
    SetupAddressWidth = 0x03,
    SetupRetransmission = 0x04,
    RFChannel = 0x05,
    RFSetup = 0x06,
    Status = 0x07,
    ObserveTX = 0x08,
    ReceivedPowerDetector = 0x09,
    RXAddressP0 = 0x0A,
    RXAddressP1 = 0x0B,
    RXAddressP2 = 0x0C,
    RXAddressP3 = 0x0D,
    RXAddressP4 = 0x0E,
    RXAddressP5 = 0x0F,
    TXAddress = 0x10,
    RXPayloadWidthPipe0 = 0x11,
    RXPayloadWidthPipe1 = 0x12,
    RXPayloadWidthPipe2 = 0x13,
    RXPayloadWidthPipe3 = 0x14,
    RXPayloadWidthPipe4 = 0x15,
    RXPayloadWidthPipe5 = 0x16,
    FIFOStatus = 0x17,
    DynamicPayloadLength = 0x1C,
    Feature = 0x1D
};

union Config {
    struct {
        uint8_t :1;
        uint8_t maskRXDR: 1;
        uint8_t maskTXDS: 1;
        uint8_t maskMaxRT: 1;
        uint8_t enableCRC: 1;
        uint8_t CRCScheme: 1;
        uint8_t powerUp: 1;
        uint8_t primRX: 1; // RX/TX
    };
    uint8_t raw;
};

union EnableAutoAck {
    struct {
        uint8_t :3;
        uint8_t pipe5: 1;
        uint8_t pipe4: 1;
        uint8_t pipe3: 1;
        uint8_t pipe2: 1;
        uint8_t pipe1: 1;
        uint8_t pipe0: 1;
    };
    uint8_t raw;
};

union EnableRXAddress {
    struct {
        uint8_t :3;
        uint8_t pipe5: 1;
        uint8_t pipe4: 1;
        uint8_t pipe3: 1;
        uint8_t pipe2: 1;
        uint8_t pipe1: 1;
        uint8_t pipe0: 1;
    };
    uint8_t raw;
};

union SetupAddressWidth {
    struct {
        uint8_t :6;
        uint8_t addressWidth: 1;
    };
    uint8_t raw;
};

union SetupRetransmission {
    struct {
        uint8_t autoRetransmitDelay: 4;
        uint8_t autoRetransmitCount: 4;
    };
    uint8_t raw;
};

union RFChannel {
    struct {
        uint8_t: 1;
        uint8_t channel: 7;
    };
    uint8_t raw;
};

union RFSetup {
    struct {
        uint8_t enableContinuousTransmit: 1;
        uint8_t: 1;
        uint8_t rfDRLow: 1;
        uint8_t pllLock: 1;
        uint8_t rfDRHigh: 1;
        uint8_t rfPower: 2;
        uint8_t :1;

    };
    uint8_t raw;
};

union Status {
    struct {
        uint8_t: 1;
        uint8_t RXDataReadyInterrupt: 1;
        uint8_t TXDataSentInterrupt: 1;
        uint8_t maxRetransmitsInterrupt: 1;
        uint8_t RXPipeNumber: 3;
        uint8_t TXFull: 1;
    };
    uint8_t raw;
};

union ObserveTX {
    struct {
        uint8_t packetLossCount: 4;
        uint8_t retransmittedCount: 4;
    };
    uint8_t raw;
};

union RPD {
    struct {
        uint8_t: 7;
        uint8_t receivedPowerDetector: 4;
    };
    uint8_t raw;
};

union RXPipeDataWidth {
    struct {
        uint8_t: 2;
        uint8_t dataWidth: 4;
    };
    uint8_t raw;
};

union FIFOStatus {
    struct {
        uint8_t: 1;
        uint8_t TXReuse: 1;
        uint8_t TXFull: 1;
        uint8_t TXEmpty: 1;
        uint8_t: 2;
        uint8_t RXFull: 1;
        uint8_t RXEmpty: 2;

    };
    uint8_t raw;
};

union DynamicPayloadLength {
    struct {
        uint8_t: 2;
        uint8_t enableForPipe0: 1;
        uint8_t enableForPipe1: 1;
        uint8_t enableForPipe2: 1;
        uint8_t enableForPipe3: 1;
        uint8_t enableForPipe4: 1;
        uint8_t enableForPipe5: 1;
    };
    uint8_t raw;
};

union Feature {
    struct {
        uint8_t: 5;
        uint8_t enableDynamicPayloadLength: 1;
        uint8_t enablePayloadWithAck: 1;
        uint8_t enableNoAckTX: 1;
    };
    uint8_t raw;
};

class NRF24 {
public:
    explicit NRF24();
    uint8_t readSingleByteRegister(uint8_t reg);

    Status getStatus();
private:
    uint8_t writeSingleByteRegister();

    uint8_t status = 0;

    uint8_t spi_xfer8(uint8_t data);
};


#endif //UART_LOGGER_NRF24_H
