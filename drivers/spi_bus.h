// ===============================================
// File:   spi_bus.h
// Desc:   Header file for spi_bus.h
// ===============================================

#ifndef SPI_BUS_H
#define SPI_BUS_H

#include <stdint.h>
#include "core/types.h"

/*
 * SPI RC3/RC4/RC5 + SS=RB3 (EEPROM)
 */
app_err_t spi_bus_init(uint32_t hz);

#endif // SPI_BUS_H
