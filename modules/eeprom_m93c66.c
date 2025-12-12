// ===============================================
// File:   eeprom_m93c66.c
// Desc:   Source file for eeprom_m93c66.c
// ===============================================

#include "eeprom_m93c66.h"
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 64000000UL
#endif

#define EEPROM_CS_HIGH()  PORTBbits.RB3 = 1
#define EEPROM_CS_LOW()   PORTBbits.RB3 = 0

// ------------------------------------------------
// Init : EWEN
// ------------------------------------------------
void eeprom_init(void)
{
    EEPROM_CS_HIGH();

    SPI1TWIDTH = 4;
    SPI1TCNTL  = 1;
    SPI1TCNTH  = 0;
    SPI1CON2bits.TXR = 1;
    SPI1CON0bits.EN  = 1;

    SPI1TXB = 0b10011000;
    while (SPI1STATUSbits.TXBE == 0);
    SPI1TXB = 0b00001111;
    while (SPI1STATUSbits.TXBE == 0);

    SPI1CON2bits.TXR = 0;
    SPI1CON0bits.EN  = 0;
    EEPROM_CS_LOW();

    __delay_ms(5);   // temps d’écriture EEPROM
}

// ------------------------------------------------
// Write 8 bits
// ------------------------------------------------
app_err_t eeprom_write_record(uint16_t addr, uint8_t val)
{
    uint8_t b1 = (0b101 << 5) | ((addr >> 4) & 0x1F);
    uint8_t b2 = ((addr & 0x0F) << 4) | ((val >> 4) & 0x0F);
    uint8_t b3 = ((val & 0x0F) << 4) | 0x0F;

    eeprom_init();   // EWEN

    EEPROM_CS_HIGH();

    SPI1TWIDTH = 4;
    SPI1TCNTL  = 2;
    SPI1TCNTH  = 0;
    SPI1CON2bits.TXR = 1;
    SPI1CON0bits.EN  = 1;

    SPI1TXB = b1; while (SPI1STATUSbits.TXBE == 0);
    SPI1TXB = b2; while (SPI1STATUSbits.TXBE == 0);
    SPI1TXB = b3; while (SPI1STATUSbits.TXBE == 0);

    SPI1CON2bits.TXR = 0;
    SPI1CON0bits.EN  = 0;
    EEPROM_CS_LOW();

    __delay_ms(5);   // temps d’écriture EEPROM

    return APP_OK;
}

// ------------------------------------------------
// Read 8 bits
// ------------------------------------------------
app_err_t eeprom_read_record(uint16_t addr, uint8_t *val)
{
    if (!val) return APP_ERR_PARAM;

    uint8_t b1 = (0b110 << 5) | ((addr >> 4) & 0x1F);
    uint8_t b2 = ((addr & 0x0F) << 4) | 0x0F;
    uint8_t v2, v3;

    EEPROM_CS_HIGH();

    SPI1TWIDTH = 0;
    SPI1TCNTL  = 3;
    SPI1TCNTH  = 0;
    SPI1CON2bits.TXR = 1;
    SPI1CON2bits.RXR = 1;
    SPI1CON0bits.EN  = 1;

    SPI1TXB = b1; while (SPI1STATUSbits.TXBE == 0); (void)SPI1RXB;
    SPI1TXB = b2; while (SPI1STATUSbits.TXBE == 0); v2 = SPI1RXB;
    SPI1TXB = 0xFF; while (SPI1STATUSbits.TXBE == 0); v3 = SPI1RXB;

    SPI1CON2bits.RXR = 0;
    SPI1CON2bits.TXR = 0;
    SPI1CON0bits.EN  = 0;
    EEPROM_CS_LOW();

    *val = (uint8_t)(((v2 & 0x0F) << 4) | ((v3 >> 4) & 0x0F));
    return APP_OK;
}
