// ===============================================
// File:   sht30.c
// Desc:   Source file for SHT30 (PIC18F26K83 / I2C2 registers)
// ===============================================

#include "sht30.h"
#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 64000000UL
#endif

#define SHT30_ADDR_WRITE  0x88
#define SHT30_ADDR_READ   0x89

// Single shot measurement, High repeatability, Clock stretching disabled
// Datasheet command: 0x2400
#define SHT30_CMD_MSB     0x24
#define SHT30_CMD_LSB     0x00

// Typical max measurement time for high repeatability ~15 ms
#define SHT30_MEAS_DELAY_MS  15

// Optional: local CRC-8 (poly 0x31, init 0xFF)
static uint8_t sht30_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

app_err_t sht30_init(void)
{
    // I2C init is assumed to be done elsewhere
    return APP_OK;
}

app_err_t sht30_read(sht30_data_t *data)
{
    if (data == 0) {
        return APP_EPARAM;
    }

    uint16_t raw_temperature = 0;
    uint16_t raw_humidity    = 0;

    uint8_t b1, b2, b3;
    uint8_t b4, b5, b6;

    // ------------------------------------------------------------
    // 1) WRITE measurement command (0x2400)
    // ------------------------------------------------------------
    I2C2CON0bits.RSEN = 0;       // no repeated start for this phase
    I2C2CNT           = 2;       // 2 bytes to transmit (cmd MSB + cmd LSB)
    I2C2ADB1          = SHT30_ADDR_WRITE;

    I2C2TXB           = SHT30_CMD_MSB;

    I2C2CON0bits.S = 1;          // START
    while (!I2C2PIRbits.SCIF);
    I2C2PIRbits.SCIF = 0;

    while (!I2C2STAT1bits.TXBE);
    I2C2TXB = SHT30_CMD_LSB;

    // Wait end of transaction (STOP / count done depends on your I2C config)
    while (!I2C2PIRbits.PCIF);
    I2C2PIRbits.PCIF = 0;

    // Wait measurement time
    __delay_ms(SHT30_MEAS_DELAY_MS);

    // ------------------------------------------------------------
    // 2) READ 6 bytes: T(MSB,LSB,CRC) + RH(MSB,LSB,CRC)
    // ------------------------------------------------------------
    I2C2CNT  = 6;
    I2C2ADB1 = SHT30_ADDR_READ;

    I2C2CON0bits.S = 1;          // START
    while (!I2C2PIRbits.SCIF);
    I2C2PIRbits.SCIF = 0;

    // Read bytes 1..5 with ACK, last byte with NACK
    I2C2CON1bits.ACKCNT = 0;     // ACK

    while (!I2C2STAT1bits.RXBF);
    b1 = I2C2RXB;

    while (!I2C2STAT1bits.RXBF);
    b2 = I2C2RXB;

    while (!I2C2STAT1bits.RXBF);
    b3 = I2C2RXB;

    while (!I2C2STAT1bits.RXBF);
    b4 = I2C2RXB;

    while (!I2C2STAT1bits.RXBF);
    b5 = I2C2RXB;

    I2C2CON1bits.ACKCNT = 1;     // NACK for last byte
    while (!I2C2STAT1bits.RXBF);
    b6 = I2C2RXB;

    while (!I2C2PIRbits.PCIF);
    I2C2PIRbits.PCIF = 0;

    // ------------------------------------------------------------
    // 3) CRC check (recommended). If you prefer to ignore CRC,
    //    comment this block.
    // ------------------------------------------------------------
    {
        uint8_t t2[2]  = { b1, b2 };
        uint8_t rh2[2] = { b4, b5 };

        if (sht30_crc8(t2, 2) != b3) {
            return APP_EIO;
        }
        if (sht30_crc8(rh2, 2) != b6) {
            return APP_EIO;
        }
    }

    // ------------------------------------------------------------
    // 4) Convert raw values
    // Temperature (°C): -45 + 175 * raw / 65535
    // Stored as °C * 100 (int16)
    // Humidity (%RH): 100 * raw / 65535
    // Stored as % * 100 (uint16)
    // ------------------------------------------------------------
    raw_temperature = ((uint16_t)b1 << 8) | b2;
    raw_humidity    = ((uint16_t)b4 << 8) | b5;

    {
        int32_t temp_c_x100 =
            -4500L + ((17500L * (int32_t)raw_temperature) / 65535L);
        data->temp_c_x100 = (int16_t)temp_c_x100;
    }

    data->rh_x100 = (uint16_t)((10000UL * (uint32_t)raw_humidity) / 65535UL);

    return APP_OK;
}
