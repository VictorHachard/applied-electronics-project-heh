// ===============================================
// File:   sht30.c
// Desc:   Source file for sht30.c
// ===============================================

#include "sht30.h"

app_err_t sht30_init(void) {
    return APP_OK;
}

app_err_t sht30_read(sht30_data_t *data) {
    if (data) {
        data->rh_x100      = 0;
        data->temp_c_x100  = 0;
    }
    return APP_OK;
}