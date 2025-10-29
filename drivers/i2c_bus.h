// ===============================================
// File:   i2c_bus.h
// Desc:   Header file for i2c_bus.h
// ===============================================

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <stdint.h>
#include "core/types.h"

/*
 * I2C RB1(SDA)/RB2(SCL) pour communication avec capteurs SHT30 et BMP280
 */
app_err_t i2c_bus_init(uint32_t hz);

app_err_t i2c2_write(uint8_t addr8w, const uint8_t *buf, uint8_t n);

app_err_t i2c2_write_read(uint8_t addr8w, const uint8_t *w, uint8_t wn,
                          uint8_t addr8r, uint8_t *r, uint8_t rn);

#endif // I2C_BUS_H
