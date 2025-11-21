// ===============================================
// File:   bmp280.c
// Desc:   Source file for bmp280.c
// ===============================================

#include "bmp280.h"

app_err_t bmp280_init(void) {
    return APP_OK;
}

app_err_t bmp280_read(bmp280_data_t *data) {
    if (data) {
        data->pressure_pa = 0;
        data->temp_c_x100 = 0;
    }
    return APP_OK;
}