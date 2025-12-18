// ===============================================
// File:   datalogger.c
// Desc:   Source file for datalogger.c
// ===============================================

#include "datalogger.h"
#include "../modules/eeprom_m93c66.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DL_EE_SAMPLE_PERIOD   0u  // P茅riode en MINUTES
#define DL_EE_START_DAY       1u
#define DL_EE_START_MONTH     2u
#define DL_EE_START_HOUR      3u
#define DL_EE_START_MIN       4u
#define DL_EE_DATA_COUNT      5u
#define DL_EE_RUNNING         6u
#define DL_EE_LOG_BASE        7u

#ifndef DL_EEPROM_TOTAL_BYTES
#define DL_EEPROM_TOTAL_BYTES 256u
#endif

#define DL_LOG_BYTES_AVAILABLE (DL_EEPROM_TOTAL_BYTES - DL_EE_LOG_BASE)
#define DL_MAX_RECORDS         (DL_LOG_BYTES_AVAILABLE / 3u)
#define DL_PERIOD_NOT_SET      0xFF

/* ============================================================================
 * CACHE RAM - Évite les lectures EEPROM répétées
 * ============================================================================ */
static dl_cfg_t g_config_cache;
static uint8_t g_cache_valid = 0;  // 0 = invalide, 1 = valide

/* ---------------------------------------------------------------------------
 * FONCTIONS DE GESTION DU CACHE
 * -------------------------------------------------------------------------- */

// Invalide le cache (force rechargement depuis EEPROM au prochain accès)
static void cache_invalidate(void) {
    g_cache_valid = 0;
}

// Charge la config depuis l'EEPROM dans le cache
static app_err_t cache_load(void) {
    app_err_t err;
    uint8_t x;

    err = eeprom_read_record(DL_EE_SAMPLE_PERIOD, &x);
    if (err != APP_OK) return err;
    g_config_cache.sample_period_min = x;

    err = eeprom_read_record(DL_EE_START_DAY, &x);
    if (err != APP_OK) return err;
    g_config_cache.start_time.day = x;

    err = eeprom_read_record(DL_EE_START_MONTH, &x);
    if (err != APP_OK) return err;
    g_config_cache.start_time.month = x;

    err = eeprom_read_record(DL_EE_START_HOUR, &x);
    if (err != APP_OK) return err;
    g_config_cache.start_time.hour = x;

    err = eeprom_read_record(DL_EE_START_MIN, &x);
    if (err != APP_OK) return err;
    g_config_cache.start_time.min = x;

    err = eeprom_read_record(DL_EE_DATA_COUNT, &x);
    if (err != APP_OK) return err;
    g_config_cache.data_count = x;

    err = eeprom_read_record(DL_EE_RUNNING, &x);
    if (err != APP_OK) return err;
    g_config_cache.running = (x != 0);

    g_cache_valid = 1;
    return APP_OK;
}

// Récupère la config (depuis cache si valide, sinon charge depuis EEPROM)
static app_err_t cache_get(dl_cfg_t *cfg) {
    if (!cfg) return APP_EPARAM;

    if (!g_cache_valid) {
        app_err_t err = cache_load();
        if (err != APP_OK) return err;
    }

    memcpy(cfg, &g_config_cache, sizeof(dl_cfg_t));
    return APP_OK;
}

// Met à jour un champ du cache ET de l'EEPROM
static app_err_t cache_update_field(uint16_t addr, uint8_t value) {
    app_err_t err = eeprom_write_record(addr, value);
    if (err != APP_OK) return err;

    // Mettre à jour le cache en RAM
    if (!g_cache_valid) {
        err = cache_load();
        if (err != APP_OK) return err;
    }

    // Mise à jour du champ correspondant
    switch (addr) {
        case DL_EE_SAMPLE_PERIOD:
            g_config_cache.sample_period_min = value;
            break;
        case DL_EE_START_DAY:
            g_config_cache.start_time.day = value;
            break;
        case DL_EE_START_MONTH:
            g_config_cache.start_time.month = value;
            break;
        case DL_EE_START_HOUR:
            g_config_cache.start_time.hour = value;
            break;
        case DL_EE_START_MIN:
            g_config_cache.start_time.min = value;
            break;
        case DL_EE_DATA_COUNT:
            g_config_cache.data_count = value;
            break;
        case DL_EE_RUNNING:
            g_config_cache.running = (value != 0);
            break;
    }

    return APP_OK;
}

/* ---------------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------------- */
static int16_t clamp_i16(int16_t v, int16_t lo, int16_t hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static uint16_t clamp_u16(uint16_t v, uint16_t lo, uint16_t hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static uint32_t udiv_round(uint32_t n, uint32_t d) {
    return (n + (d / 2u)) / d;
}

/* ---------------------------------------------------------------------------
 * Configuration (API publique)
 * -------------------------------------------------------------------------- */

app_err_t dl_reset_config(void) {
    app_err_t err;

    err = cache_update_field(DL_EE_SAMPLE_PERIOD, DL_PERIOD_NOT_SET);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_DAY, 0);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_MONTH, 0);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_HOUR, 0);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_MIN, 0);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_DATA_COUNT, 0);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_RUNNING, 0);
    if (err != APP_OK) return err;

    return APP_OK;
}

app_err_t dl_set_sample_period_min(uint8_t period_min) {
    if (period_min == 0 || period_min == DL_PERIOD_NOT_SET) {
        return APP_ENCONF;
    }
    return cache_update_field(DL_EE_SAMPLE_PERIOD, period_min);
}

app_err_t dl_stop(void) {
    return cache_update_field(DL_EE_RUNNING, 0);
}

app_err_t dl_set_running(rtc_time_t *start_time) {
    if (!start_time) return APP_EPARAM;

    app_err_t err;
    dl_cfg_t cfg;

    // Vérifier qu'une période est configurée (utilise le cache)
    err = cache_get(&cfg);
    if (err != APP_OK) return err;

    if (cfg.sample_period_min == 0 || cfg.sample_period_min == DL_PERIOD_NOT_SET) {
        return APP_ENOTCONFIG;
    }

    // Enregistrer l'heure de démarrage
    err = cache_update_field(DL_EE_START_DAY, start_time->day);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_MONTH, start_time->month);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_HOUR, start_time->hour);
    if (err != APP_OK) return err;

    err = cache_update_field(DL_EE_START_MIN, start_time->min);
    if (err != APP_OK) return err;

    // Reset compteur
    err = cache_update_field(DL_EE_DATA_COUNT, 0);
    if (err != APP_OK) return err;

    // Activer le datalogger
    err = cache_update_field(DL_EE_RUNNING, 1);
    if (err != APP_OK) return err;

    return APP_OK;
}

app_err_t dl_get_config(dl_cfg_t *cfg) {
    return cache_get(cfg);  // ✅ Utilise le cache !
}

/* ---------------------------------------------------------------------------
 * Réduction / Dé-réduction
 * -------------------------------------------------------------------------- */

app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out) {
    if (!in || !out) return APP_EPARAM;

    int16_t t = in->t_c_x100;
    t = clamp_i16(t, -4000, 8750);
    uint16_t t_shift = (uint16_t)(t + 4000);
    uint8_t t8 = (uint8_t)udiv_round((uint32_t)t_shift, 50u);
    out->t8 = t8;

    uint16_t rh = clamp_u16(in->rh_x100, 0, 10000);
    uint8_t rh8 = (uint8_t)udiv_round((uint32_t)rh, 50u);
    if (rh8 > 200u) rh8 = 200u;
    out->rh8 = rh8;

    int32_t p_pa = clamp_i32(in->p_pa, 30000, 110000);
    int32_t p_hPa = (p_pa + 50) / 100;
    if (p_hPa < 800)  p_hPa = 800;
    if (p_hPa > 1200) p_hPa = 1200;

    uint8_t p8 = (uint8_t)udiv_round((uint32_t)(p_hPa - 800), 2u);
    if (p8 > 200u) p8 = 200u;
    out->p8 = p8;

    return APP_OK;
}

void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out) {
    if (!in || !out) return;

    out->t_c_x100 = (int16_t)((int32_t)in->t8 * 50 - 4000);
    out->rh_x100 = (uint16_t)((uint32_t)in->rh8 * 50);
    out->p_pa = ((int32_t)in->p8 * 2 + 800) * 100;
}

/* ---------------------------------------------------------------------------
 * Log: push / read
 * -------------------------------------------------------------------------- */

app_err_t dl_push_record(const sensor_data_t *rec) {
    if (!rec) return APP_EPARAM;

    app_err_t err;
    dl_cfg_t cfg;

    // ✅ Utilise le cache au lieu de relire l'EEPROM
    err = cache_get(&cfg);
    if (err != APP_OK) return err;

    if (!cfg.running) {
        return APP_ENOTRUNNING;
    }

    if (cfg.data_count >= (uint8_t)DL_MAX_RECORDS) {
        return APP_EFULL;
    }

    // Réduire les données
    sensor_reduced_t r;
    err = sensor_reduce(rec, &r);
    if (err != APP_OK) return err;

    // Écrire dans l'EEPROM
    uint16_t base = (uint16_t)(DL_EE_LOG_BASE + ((uint16_t)cfg.data_count * 3u));

    err = eeprom_write_record(base + 0u, r.t8);
    if (err != APP_OK) return err;

    err = eeprom_write_record(base + 1u, r.rh8);
    if (err != APP_OK) return err;

    err = eeprom_write_record(base + 2u, r.p8);
    if (err != APP_OK) return err;

    // ✅ Incrémenter le compteur (cache + EEPROM)
    err = cache_update_field(DL_EE_DATA_COUNT, cfg.data_count + 1);
    if (err != APP_OK) return err;

    return APP_OK;
}

app_err_t dl_read(uint16_t index, sensor_data_t *rec) {
    if (!rec) return APP_EPARAM;

    app_err_t err;
    dl_cfg_t cfg;

    // ✅ Utilise le cache
    err = cache_get(&cfg);
    if (err != APP_OK) return err;

    if (index >= cfg.data_count) return APP_ENOENT;

    uint16_t base = (uint16_t)(DL_EE_LOG_BASE + (index * 3u));

    sensor_reduced_t r;
    err = eeprom_read_record(base + 0u, &r.t8);
    if (err != APP_OK) return err;

    err = eeprom_read_record(base + 1u, &r.rh8);
    if (err != APP_OK) return err;

    err = eeprom_read_record(base + 2u, &r.p8);
    if (err != APP_OK) return err;

    sensor_dereduce(&r, rec);
    return APP_OK;
}

app_err_t dl_get_record_count(uint8_t *count) {
    if (!count) return APP_EPARAM;
    
    dl_cfg_t cfg;
    app_err_t err = cache_get(&cfg);
    if (err != APP_OK) return err;
    
    *count = cfg.data_count;
    return APP_OK;
}