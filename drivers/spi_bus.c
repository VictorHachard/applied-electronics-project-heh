// ===============================================
// File:   spi_bus.c
// Desc:   Driver SPI pour PIC18F26K83 (SPI1)
//         Communication avec EEPROM M93C66
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "spi_bus.h"
#include <xc.h>
#include <stdint.h>

// ===============================================
// FONCTION : spi_bus_init
// ===============================================
// Initialise le module SPI1 en mode Master
//
// CONFIGURATION :
//   - Mode Master
//   - État IDLE des bus = 0
//   - Source CLK = MFINTOSC
//   - BAUD = 100 (à calculer selon besoin)
//
// ===============================================
app_err_t spi_bus_init() {
    // Configuration SPI1
    SPI1CON0 = 0b00000010;  // Mode master
    SPI1CON1 = 0b11000000;  // État IDLE des bus = 0
    
    SPI1TWIDTH = 0;         // Par défaut 8 bits
    SPI1TCNTL  = 0;
    SPI1TCNTH  = 0;
    
    SPI1CLK  = 0b00000010;  // Source de la CLK = MFINTOSC
    SPI1BAUD = 100;         // Diviseur de fréquence
    
    // CS inactif (HIGH)
    LATBbits.LATB3 = 1;
    
    return APP_OK;
}