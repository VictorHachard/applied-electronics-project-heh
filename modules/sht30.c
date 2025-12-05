// ===============================================
// File:   sht30.c
// Desc:   Source file for sht30.c
// ===============================================

#include "sht30.h"
#include <xc.h>

#define SHT30_ADDR_WRITE  0x88 
#define SHT30_ADDR_READ   0x89

#define SHT30_CMD_MSB     0xE0
#define SHT30_CMD_LSB     0x00

app_err_t sht30_init(void)
{
    return APP_OK;
}

app_err_t sht30_read(sht30_data_t *data)
{
    if (data == 0) {
        return APP_EPARAM;
    }

    uint16_t raw_temperature;
    uint16_t raw_humidity;
    unsigned char b1, b2, b3;
    unsigned char b4, b5, b6;

    I2C2CON0bits.RSEN = 0;
    I2C2CNT           = 2;  
    I2C2ADB1          = SHT30_ADDR_WRITE;
    I2C2TXB           = SHT30_CMD_MSB;

    I2C2CON0bits.S = 1;
    while(!I2C2PIRbits.SCIF);
    I2C2PIRbits.SCIF = 0;

    while(!I2C2STAT1bits.TXBE);
    I2C2TXB = SHT30_CMD_LSB;

    while(!I2C2PIRbits.PCIF);
    I2C2PIRbits.PCIF = 0;

    I2C2CNT  = 6;
    I2C2ADB1 = SHT30_ADDR_READ;

    I2C2CON0bits.S = 1;
    while(!I2C2PIRbits.SCIF);
    I2C2PIRbits.SCIF = 0;
    I2C2CON1bits.ACKCNT = 0;
    while(!I2C2STAT1bits.RXBF);
    b1 = I2C2RXB;
    while(!I2C2STAT1bits.RXBF);
    b2 = I2C2RXB;
    while(!I2C2STAT1bits.RXBF);
    b3 = I2C2RXB;
    while(!I2C2STAT1bits.RXBF);
    b4 = I2C2RXB;
    while(!I2C2STAT1bits.RXBF);
    b5 = I2C2RXB;
    I2C2CON1bits.ACKCNT = 1;
    while(!I2C2STAT1bits.RXBF);
    b6 = I2C2RXB;
    while(!I2C2PIRbits.PCIF);
    I2C2PIRbits.PCIF = 0;

    // Reconstruction des valeurs brutes 

    raw_temperature = ((uint16_t)b1 << 8) | b2;
    raw_humidity    = ((uint16_t)b4 << 8) | b5;
    {
        int32_t temp_c_x100 =
            -4500 + ((17500L * (int32_t)raw_temperature) / 65535L);
        data->temp_c_x100 = (int16_t)temp_c_x100;
    }
    data->rh_x100 = (uint16_t)((10000UL * (uint32_t)raw_humidity) / 65535UL);

    return APP_OK;
}