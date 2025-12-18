// ===============================================
// File:   bluetooth_proto.c
// Desc:   Source file for bluetooth_proto.c
// ===============================================

#include "bluetooth_proto.h"
#include "datalogger.h"
#include "../drivers/uart_bt.h"

// ===============================================
// FONCTIONS D'ENVOI
// ===============================================

// Envoie un int32 en décimal
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

// Envoie 2 chiffres avec zero padding
static void put2d_bt(uint8_t val) {
    uart_bt_putc('0' + val / 10);
    uart_bt_putc('0' + val % 10);
}

// ===============================================
// INDEX POUR LECTURE SÉQUENTIELLE
// ===============================================

static uint8_t g_idx = 0;

// ===============================================
// TRAITEMENT DES COMMANDES
// ===============================================

void bluetooth_handle_rx(char cmd) {
    dl_cfg_t cfg;
    sensor_data_t data;

    switch (cmd) {

        // ===== C : Configuration =====
        // Réponse: "HHMM;DDMM;period\r\n"
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

        // ===== N : Nombre de records =====
        // Réponse: "count\r\n"
        case 'N':
        case 'n':
            if (dl_get_config(&cfg) == APP_OK) {
                puti_bt(cfg.data_count);
                uart_bt_puts("\r\n");
            } else {
                uart_bt_puts("0\r\n");
            }
            break;

        // ===== R : Reset index =====
        // Réponse: "OK\r\n"
        case 'R':
        case 'r':
            g_idx = 0;
            uart_bt_puts("OK\r\n");
            break;

        // ===== A : Avancer (prochain record) =====
        // Réponse: "idx;T;H;P\r\n"
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

        // ===== Commande inconnue =====
        default:
            uart_bt_puts("ERR\r\n");
            break;
    }
}

// ===============================================
// INITIALISATION
// ===============================================

app_err_t bluetooth_init(void) {
    g_idx = 0;
    return APP_OK;
}