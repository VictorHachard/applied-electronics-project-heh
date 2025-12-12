// ===============================================
// File:   eeprom_m93c66.h
// Desc:   Header file for eeprom_m93c66.h
// ===============================================

#ifndef EEPROM_M93C66_H
#define EEPROM_M93C66_H

#include <stdint.h>
#include "../core/types.h"

void eeprom_init(void);

app_err_t eeprom_write_record(uint16_t addr, uint8_t val);

app_err_t eeprom_read_record(uint16_t addr, uint8_t *val);

app_err_t eeprom_chip_erase(void);

#endif // EEPROM_M93C66_H
