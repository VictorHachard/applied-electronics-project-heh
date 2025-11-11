// ===============================================
// File:   i2c_bus.h
// Desc:   Driver I2C pour PIC18F26K83
//         Communication avec RTC DS1307, SHT30, BMP280
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <stdint.h>
#include "../core/types.h"

// Initialisation du bus I2C2 en mode Master
app_err_t i2c_bus_init();

// Écriture de données sur le bus I2C2
app_err_t i2c2_write(uint8_t addr8w, const uint8_t *buf, uint8_t n);

// Écriture puis lecture sur le bus I2C2 (RESTART)
app_err_t i2c2_write_read(uint8_t addr8w, const uint8_t *w, uint8_t wn,
                          uint8_t addr8r, uint8_t *r, uint8_t rn);

#endif // I2C_BUS_H
