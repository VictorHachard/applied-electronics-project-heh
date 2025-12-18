# 1 "../modules/datalogger.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "../modules/datalogger.c" 2





# 1 "../modules/datalogger.h" 1








# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned __int24 uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef __int24 intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 194 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 235 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 10 "../modules/datalogger.h" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdbool.h" 1 3
# 11 "../modules/datalogger.h" 2
# 1 "../modules/../core/types.h" 1
# 16 "../modules/../core/types.h"
typedef struct {
  uint8_t hour, min;
  uint8_t day, month;
} rtc_time_t;
# 34 "../modules/../core/types.h"
typedef struct {
  int32_t pressure_pa;
  int16_t temp_c_x100;
} bmp280_data_t;
# 52 "../modules/../core/types.h"
typedef struct {
  uint16_t rh_x100;
  int16_t temp_c_x100;
} sht30_data_t;
# 68 "../modules/../core/types.h"
typedef struct {
  int16_t t_c_x100;
  uint16_t rh_x100;
  int32_t p_pa;
} sensor_data_t;




typedef enum {
  APP_OK = 0,
  APP_EBUS,
  APP_EDEV,
  APP_EPARAM,
  APP_ENOENT,
  APP_ENCONF,
  APP_EIO,
  APP_EFULL
} app_err_t;
# 12 "../modules/datalogger.h" 2



typedef struct {
  uint8_t sample_period_s;
  uint8_t data_count;
  rtc_time_t start_time;
  _Bool running;
} dl_cfg_t;

app_err_t dl_reset_config(void);

app_err_t dl_set_sample_period_s(uint8_t period_s);

app_err_t dl_set_running(rtc_time_t *start_time);

app_err_t dl_get_config(dl_cfg_t *cfg);
# 58 "../modules/datalogger.h"
typedef struct {
  uint8_t t8;
  uint8_t rh8;
  uint8_t p8;
} sensor_reduced_t;


app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out);







void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out);



app_err_t dl_push_record(const sensor_data_t *rec);

app_err_t dl_read(uint16_t index, sensor_data_t *rec);
# 7 "../modules/datalogger.c" 2
# 1 "../modules/../modules/eeprom_m93c66.h" 1
# 12 "../modules/../modules/eeprom_m93c66.h"
void eeprom_init(void);

app_err_t eeprom_write_record(uint16_t addr, uint8_t val);

app_err_t eeprom_read_record(uint16_t addr, uint8_t *val);

app_err_t eeprom_chip_erase(void);
# 8 "../modules/datalogger.c" 2
# 38 "../modules/datalogger.c"
static uint8_t G_data_count = 0;




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





app_err_t dl_reset_config(void)
{
  app_err_t err;

  err = eeprom_write_record(0u, (uint8_t)300u);
  if (err != APP_OK) return err;

  err = eeprom_write_record(1u, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(2u, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(3u, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(4u, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(5u, 0);
  if (err != APP_OK) return err;

  err = eeprom_write_record(6u, 0);
  if (err != APP_OK) return err;

  G_data_count = 0;
  return APP_OK;
}

app_err_t dl_set_sample_period_s(uint8_t period_s)
{
  if (period_s == 0) return APP_ENCONF;
  return eeprom_write_record(0u, period_s);
}

app_err_t dl_set_running(rtc_time_t *start_time)
{
  if (!start_time) return APP_EPARAM;

  app_err_t err;

  err = eeprom_write_record(1u, start_time->day);
  if (err != APP_OK) return err;

  err = eeprom_write_record(2u, start_time->month);
  if (err != APP_OK) return err;

  err = eeprom_write_record(3u, start_time->hour);
  if (err != APP_OK) return err;

  err = eeprom_write_record(4u, start_time->min);
  if (err != APP_OK) return err;


  err = eeprom_write_record(5u, 0);
  if (err != APP_OK) return err;

  G_data_count = 0;


  err = eeprom_write_record(6u, 1);
  if (err != APP_OK) return err;

  return APP_OK;
}

app_err_t dl_get_config(dl_cfg_t *cfg)
{
  if (!cfg) return APP_EPARAM;

  app_err_t err;
  uint8_t x;

  err = eeprom_read_record(0u, &x);
  if (err != APP_OK) return err;
  cfg->sample_period_s = x;

  err = eeprom_read_record(1u, &x);
  if (err != APP_OK) return err;
  cfg->start_time.day = x;

  err = eeprom_read_record(2u, &x);
  if (err != APP_OK) return err;
  cfg->start_time.month = x;

  err = eeprom_read_record(3u, &x);
  if (err != APP_OK) return err;
  cfg->start_time.hour = x;

  err = eeprom_read_record(4u, &x);
  if (err != APP_OK) return err;
  cfg->start_time.min = x;

  err = eeprom_read_record(5u, &x);
  if (err != APP_OK) return err;
  cfg->data_count = x;

  err = eeprom_read_record(6u, &x);
  if (err != APP_OK) return err;
  cfg->running = (x != 0);


  G_data_count = cfg->data_count;

  return APP_OK;
}
# 190 "../modules/datalogger.c"
app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out)
{
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
  if (p_hPa < 800) p_hPa = 800;
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





static app_err_t dl_is_running(_Bool *running)
{
  if (!running) return APP_EPARAM;
  uint8_t x = 0;
  app_err_t err = eeprom_read_record(6u, &x);
  if (err != APP_OK) return err;
  *running = (x != 0);
  return APP_OK;
}

static app_err_t dl_get_count(uint8_t *count)
{
  if (!count) return APP_EPARAM;
  app_err_t err = eeprom_read_record(5u, count);
  if (err != APP_OK) return err;
  G_data_count = *count;
  return APP_OK;
}

static app_err_t dl_set_count(uint8_t count)
{
  G_data_count = count;
  return eeprom_write_record(5u, count);
}

app_err_t dl_push_record(const sensor_data_t *rec)
{
  if (!rec) return APP_EPARAM;

  app_err_t err;

  uint8_t count = 0;
  err = dl_get_count(&count);
  if (err != APP_OK) return err;

  if (count >= (uint8_t)((256u - 7u) / 3u)) return APP_EFULL;

  sensor_reduced_t r;
  err = sensor_reduce(rec, &r);
  if (err != APP_OK) return err;


  uint16_t base = (uint16_t)(7u + ((uint16_t)count * 3u));

  err = eeprom_write_record(base + 0u, r.t8);
  if (err != APP_OK) return err;

  err = eeprom_write_record(base + 1u, r.rh8);
  if (err != APP_OK) return err;

  err = eeprom_write_record(base + 2u, r.p8);
  if (err != APP_OK) return err;


  count++;
  err = dl_set_count(count);
  if (err != APP_OK) return err;

  return APP_OK;
}

app_err_t dl_read(uint16_t index, sensor_data_t *rec)
{
  if (!rec) return APP_EPARAM;

  uint8_t count = 0;
  app_err_t err = dl_get_count(&count);
  if (err != APP_OK) return err;

  if (index >= count) return APP_ENOENT;

  uint16_t base = (uint16_t)(7u + (index * 3u));

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
