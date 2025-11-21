// ===============================================
// File:   datalogger.c
// Desc:   Source file for datalogger.c
// ===============================================

#include "datalogger.h"

app_err_t dl_get_config(dl_cfg_t *cfg) {
    if (cfg) {
        cfg->sample_period_s = 10;
        cfg->running         = false;
    }
    return APP_OK;
}

void dl_init(const dl_cfg_t *cfg) {
    (void)cfg;
}

uint16_t dl_get_sample_period_s(void) {
    return 10;   // 10 s par défaut
}

bool dl_is_running(void) {
    return false;
}

app_err_t dl_push_record(const sensor_data_t *data) {
    (void)data;
    return APP_OK;
}

void dl_stop(void) {}