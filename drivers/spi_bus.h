// ===============================================
// File:   spi_bus.h
// Desc:   Driver SPI pour PIC18F26K83
//         Communication avec EEPROM M93C66
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef SPI_BUS_H
#define SPI_BUS_H

#include <stdint.h>
#include "../core/types.h"

// Initialisation du bus SPI1 en mode Master
app_err_t spi_bus_init();

#endif // SPI_BUS_H
