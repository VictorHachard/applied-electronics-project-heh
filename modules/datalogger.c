// ===============================================
// File:   datalogger.c
// Desc:   Source file for datalogger.c
// ===============================================

#include "datalogger.h"
#include "../modules/eeprom_m93c66.h"
#include <stdint.h>
#include <stdbool.h>

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

/* Marqueur pour p茅riode non configur茅e */
#define DL_PERIOD_NOT_SET     0xFF

static uint8_t G_data_count = 0;

/* ---------------------------------------------------------------------------
 * Helpers
 * -------------------------------------------------------------------------- */
static int16_t clamp_i16(int16_t v, int16_t lo, int16_t hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static int32_t clamp_i32(int32_t v, int32_t lo, int32_t hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static uint16_t clamp_u16(uint16_t v, uint16_t lo, uint16_t hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static uint32_t udiv_round(uint32_t n, uint32_t d)
{
  return (n + (d / 2u)) / d;
}

/* ---------------------------------------------------------------------------
 * Configuration
 * -------------------------------------------------------------------------- */

app_err_t dl_reset_config(void)
{
  app_err_t err;

  /* P茅riode NON configur茅e par d茅faut */
  err = eeprom_write_record(DL_EE_SAMPLE_PERIOD, DL_PERIOD_NOT_SET);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_DAY,   0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_MONTH, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_HOUR,  0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_MIN,   0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_DATA_COUNT,  0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_RUNNING,     0);
  if (err != APP_OK) return err;

  G_data_count = 0;
  return APP_OK;
}

app_err_t dl_set_sample_period_min(uint8_t period_min)
{
  /* P茅riode doit 锚tre entre 1 et 254 minutes */
  if (period_min == 0 || period_min == DL_PERIOD_NOT_SET) {
    return APP_ENCONF;
  }
  return eeprom_write_record(DL_EE_SAMPLE_PERIOD, period_min);
}

app_err_t dl_stop(void)
{
  return eeprom_write_record(DL_EE_RUNNING, 0);
}

app_err_t dl_set_running(rtc_time_t *start_time)
{
  if (!start_time) return APP_EPARAM;

  app_err_t err;
  uint8_t period;

  /* V茅rifier qu'une p茅riode est configur茅e */
  err = eeprom_read_record(DL_EE_SAMPLE_PERIOD, &period);
  if (err != APP_OK) return err;
  
  if (period == 0 || period == DL_PERIOD_NOT_SET) {
    return APP_ENOTCONFIG;
  }

  /* Enregistrer l'heure de d茅marrage */
  err = eeprom_write_record(DL_EE_START_DAY,   start_time->day);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_MONTH, start_time->month);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_HOUR,  start_time->hour);
  if (err != APP_OK) return err;

  err = eeprom_write_record(DL_EE_START_MIN,   start_time->min);
  if (err != APP_OK) return err;

  /* Reset compteur */
  err = eeprom_write_record(DL_EE_DATA_COUNT, 0);
  if (err != APP_OK) return err;

  G_data_count = 0;

  /* Activer le datalogger */
  err = eeprom_write_record(DL_EE_RUNNING, 1);
  if (err != APP_OK) return err;

  return APP_OK;
}

app_err_t dl_get_config(dl_cfg_t *cfg)
{
  if (!cfg) return APP_EPARAM;

  app_err_t err;
  uint8_t x;

  err = eeprom_read_record(DL_EE_SAMPLE_PERIOD, &x);
  if (err != APP_OK) return err;
  cfg->sample_period_min = x;

  err = eeprom_read_record(DL_EE_START_DAY, &x);
  if (err != APP_OK) return err;
  cfg->start_time.day = x;

  err = eeprom_read_record(DL_EE_START_MONTH, &x);
  if (err != APP_OK) return err;
  cfg->start_time.month = x;

  err = eeprom_read_record(DL_EE_START_HOUR, &x);
  if (err != APP_OK) return err;
  cfg->start_time.hour = x;

  err = eeprom_read_record(DL_EE_START_MIN, &x);
  if (err != APP_OK) return err;
  cfg->start_time.min = x;

  err = eeprom_read_record(DL_EE_DATA_COUNT, &x);
  if (err != APP_OK) return err;
  cfg->data_count = x;

  err = eeprom_read_record(DL_EE_RUNNING, &x);
  if (err != APP_OK) return err;
  cfg->running = (x != 0);

  G_data_count = cfg->data_count;

  return APP_OK;
}

/* ---------------------------------------------------------------------------
 * R茅duction / D茅-r茅duction
 * -------------------------------------------------------------------------- */

app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out)
{
  if (!in || !out) return APP_EPARAM;

  /* Temp茅rature */
  int16_t t = in->t_c_x100;
  t = clamp_i16(t, -4000, 8750);
  uint16_t t_shift = (uint16_t)(t + 4000);
  uint8_t t8 = (uint8_t)udiv_round((uint32_t)t_shift, 50u);
  out->t8 = t8;

  /* Humidit茅 */
  uint16_t rh = clamp_u16(in->rh_x100, 0, 10000);
  uint8_t rh8 = (uint8_t)udiv_round((uint32_t)rh, 50u);
  if (rh8 > 200u) rh8 = 200u;
  out->rh8 = rh8;

  /* Pression */
  int32_t p_pa = clamp_i32(in->p_pa, 30000, 110000);
  int32_t p_hPa = (p_pa + 50) / 100;
  if (p_hPa < 800)  p_hPa = 800;
  if (p_hPa > 1200) p_hPa = 1200;

  uint8_t p8 = (uint8_t)udiv_round((uint32_t)(p_hPa - 800), 2u);
  if (p8 > 200u) p8 = 200u;
  out->p8 = p8;

  return APP_OK;
}

void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out)
{
  if (!in || !out) return;

  out->t_c_x100 = (int16_t)((int32_t)in->t8 * 50 - 4000);
  out->rh_x100 = (uint16_t)((uint32_t)in->rh8 * 50);
  out->p_pa = ((int32_t)in->p8 * 2 + 800) * 100;
}

/* ---------------------------------------------------------------------------
 * Log: push / read
 * -------------------------------------------------------------------------- */

app_err_t dl_push_record(const sensor_data_t *rec)
{
  if (!rec) return APP_EPARAM;

  app_err_t err;
  uint8_t running = 0;

  /* V茅rifier que le datalogger est actif */
  err = eeprom_read_record(DL_EE_RUNNING, &running);
  if (err != APP_OK) return err;
  
  if (running == 0) {
    return APP_ENOTRUNNING;
  }

  /* Obtenir le nombre d'enregistrements */
  uint8_t count = 0;
  err = eeprom_read_record(DL_EE_DATA_COUNT, &count);
  if (err != APP_OK) return err;

  if (count >= (uint8_t)DL_MAX_RECORDS) return APP_EFULL;

  /* R茅duire les donn茅es */
  sensor_reduced_t r;
  err = sensor_reduce(rec, &r);
  if (err != APP_OK) return err;

  /* 脡crire dans l'EEPROM */
  uint16_t base = (uint16_t)(DL_EE_LOG_BASE + ((uint16_t)count * 3u));

  err = eeprom_write_record(base + 0u, r.t8);
  if (err != APP_OK) return err;

  err = eeprom_write_record(base + 1u, r.rh8);
  if (err != APP_OK) return err;

  err = eeprom_write_record(base + 2u, r.p8);
  if (err != APP_OK) return err;

  /* Incr茅menter le compteur */
  count++;
  err = eeprom_write_record(DL_EE_DATA_COUNT, count);
  if (err != APP_OK) return err;

  G_data_count = count;

  return APP_OK;
}

app_err_t dl_read(uint16_t index, sensor_data_t *rec)
{
  if (!rec) return APP_EPARAM;

  uint8_t count = 0;
  app_err_t err = eeprom_read_record(DL_EE_DATA_COUNT, &count);
  if (err != APP_OK) return err;

  if (index >= count) return APP_ENOENT;

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

app_err_t dl_get_record_count(uint8_t *count)
{
  if (!count) return APP_EPARAM;
  return eeprom_read_record(DL_EE_DATA_COUNT, count);
}