// ===============================================
// File:   bmp280.h
// Desc:   Header file for bmp280.h
// ===============================================

#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include "../core/types.h"

// Initialisation du capteur BMP280
app_err_t bmp280_init(void);

// Lecture des données brutes depuis le capteur BMP280
app_err_t bmp280_read(bmp280_data_t* data);

#endif // BMP280_H
