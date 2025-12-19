# 1 "../modules/bluetooth_proto.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "../modules/bluetooth_proto.c" 2





# 1 "../modules/bluetooth_proto.h" 1








# 1 "../modules/../core/types.h" 1
# 10 "../modules/../core/types.h"
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
# 11 "../modules/../core/types.h" 2





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
    APP_ERR = 1,
    APP_EPARAM = 2,
    APP_EBUS = 3,
    APP_EDEV = 4,
    APP_EIO = 5,
    APP_EFULL = 6,
    APP_ENOENT = 7,
    APP_ENCONF = 8,
    APP_ERR_PARAM = 9,
    APP_ENOTCONFIG = 10,
    APP_ENOTRUNNING = 11
} app_err_t;
# 10 "../modules/bluetooth_proto.h" 2


app_err_t bluetooth_init(void);



void bluetooth_handle_rx(char cmd);
# 7 "../modules/bluetooth_proto.c" 2
# 1 "../modules/datalogger.h" 1
# 10 "../modules/datalogger.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdbool.h" 1 3
# 11 "../modules/datalogger.h" 2




typedef struct {
  uint8_t sample_period_min;
  uint8_t data_count;
  rtc_time_t start_time;
  _Bool running;
} dl_cfg_t;

app_err_t dl_reset_config(void);

app_err_t dl_set_sample_period_min(uint8_t period_s);

app_err_t dl_set_running(rtc_time_t *start_time);

app_err_t dl_stop(void);

app_err_t dl_get_config(dl_cfg_t *cfg);
# 60 "../modules/datalogger.h"
typedef struct {
  uint8_t t8;
  uint8_t rh8;
  uint8_t p8;
} sensor_reduced_t;


app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out);







void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out);



app_err_t dl_push_record(const sensor_data_t *rec);

app_err_t dl_read(uint16_t index, sensor_data_t *rec);
# 8 "../modules/bluetooth_proto.c" 2
# 1 "../modules/../drivers/uart_bt.h" 1
# 13 "../modules/../drivers/uart_bt.h"
app_err_t uart_bt_init(uint32_t baud);


void uart_bt_putc(char c);


void uart_bt_puts(const char* s);
# 9 "../modules/bluetooth_proto.c" 2






static void puti_bt(int32_t val) {
    char buf[12];
    uint8_t i = 0, neg = 0;

    if (val < 0) { neg = 1; val = -val; }
    if (val == 0) { uart_bt_putc('0'); return; }

    while (val > 0) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg) uart_bt_putc('-');
    while (i > 0) uart_bt_putc(buf[--i]);
}


static void put2d_bt(uint8_t val) {
    uart_bt_putc('0' + val / 10);
    uart_bt_putc('0' + val % 10);
}





static uint8_t g_idx = 0;





void bluetooth_handle_rx(char cmd) {
    dl_cfg_t cfg;
    sensor_data_t data;

    switch (cmd) {



        case 'C':
        case 'c':
            if (dl_get_config(&cfg) == APP_OK) {
                put2d_bt(cfg.start_time.hour);
                put2d_bt(cfg.start_time.min);
                uart_bt_putc(';');
                put2d_bt(cfg.start_time.day);
                put2d_bt(cfg.start_time.month);
                uart_bt_putc(';');
                puti_bt(cfg.sample_period_min);
                uart_bt_puts("\r\n");
            } else {
                uart_bt_puts("ERR\r\n");
            }
            break;



        case 'N':
        case 'n':
            if (dl_get_config(&cfg) == APP_OK) {
                puti_bt(cfg.data_count);
                uart_bt_puts("\r\n");
            } else {
                uart_bt_puts("0\r\n");
            }
            break;



        case 'R':
        case 'r':
            g_idx = 0;
            uart_bt_puts("OK\r\n");
            break;



        case 'A':
        case 'a':
            if (dl_get_config(&cfg) != APP_OK || cfg.data_count == 0) {
                uart_bt_puts("ERR\r\n");
                break;
            }
            if (g_idx >= cfg.data_count) {
                g_idx = 0;
            }
            if (dl_read(g_idx, &data) == APP_OK) {
                puti_bt(g_idx);
                uart_bt_putc(';');
                puti_bt(data.t_c_x100);
                uart_bt_putc(';');
                puti_bt(data.rh_x100);
                uart_bt_putc(';');
                puti_bt(data.p_pa);
                uart_bt_puts("\r\n");
                g_idx++;
            } else {
                uart_bt_puts("ERR\r\n");
            }
            break;


        default:
            uart_bt_puts("ERR\r\n");
            break;
    }
}





app_err_t bluetooth_init(void) {
    g_idx = 0;
    return APP_OK;
}
