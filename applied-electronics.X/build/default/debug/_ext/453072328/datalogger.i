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
  uint8_t hour, min, sec;
  uint8_t day, month;
  uint16_t year;
} rtc_time_t;
# 35 "../modules/../core/types.h"
typedef struct {
  int32_t pressure_pa;
  int16_t temp_c_x100;
} bmp280_data_t;
# 53 "../modules/../core/types.h"
typedef struct {
  uint16_t rh_x100;
  int16_t temp_c_x100;
} sht30_data_t;
# 69 "../modules/../core/types.h"
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
  APP_ENCONF,
  APP_EFULL
} app_err_t;
# 12 "../modules/datalogger.h" 2



typedef struct {
  uint16_t sample_period_s;
  _Bool running;
} dl_cfg_t;

void dl_init(const dl_cfg_t *cfg);

app_err_t dl_get_config(dl_cfg_t *cfg);

uint16_t dl_get_sample_period_s(void);

_Bool dl_is_running(void);

void dl_start(void);

void dl_stop(void);

void dl_clear(void);
# 62 "../modules/datalogger.h"
typedef struct {
  uint8_t t8;
  uint8_t rh8;
  uint8_t p8;
} sensor_reduced_t;


app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out);







void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out);
# 122 "../modules/datalogger.h"
typedef struct __attribute__((packed)) {
    uint8_t t0_min_le[3];
    uint8_t dt_s;
    uint8_t flags;
    uint8_t count;
} dl_header_t;



app_err_t dl_push_record(const sensor_data_t *rec);

uint16_t dl_count(void);

app_err_t dl_read(uint16_t index, sensor_data_t *rec);
# 7 "../modules/datalogger.c" 2

app_err_t dl_get_config(dl_cfg_t *cfg) {
    if (cfg) {
        cfg->sample_period_s = 10;
        cfg->running = 0;
    }
    return APP_OK;
}

void dl_init(const dl_cfg_t *cfg) {
    (void)cfg;
}

uint16_t dl_get_sample_period_s(void) {
    return 10;
}

_Bool dl_is_running(void) {
    return 0;
}

app_err_t dl_push_record(const sensor_data_t *data) {
    (void)data;
    return APP_OK;
}

void dl_stop(void) {}
