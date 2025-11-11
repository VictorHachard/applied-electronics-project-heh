// ===============================================
// File:   sht30.h
// Desc:   Header file for sht30.h
// ===============================================

#ifndef SHT30_H
#define SHT30_H

#include <stdint.h>
#include "../core/types.h"

// Initialisation du capteur SHT30
app_err_t sht30_init(void);

// Lecture des données brutes depuis le capteur SHT30
app_err_t sht30_read(sht30_data_t* data);

#endif // SHT30_H
