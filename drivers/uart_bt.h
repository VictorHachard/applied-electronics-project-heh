// ===============================================
// File:   uart_bt.h
// Desc:   Header file for uart_bt.h
// ===============================================

#ifndef UART_BT_H
#define UART_BT_H

#include <stdint.h>
#include "../core/types.h"

// UART 2 (RB6/RX2, RB7/TX2) pour communication Bluetooth
app_err_t uart_bt_init(uint32_t baud);

// Envoi des données capteurs via Bluetooth
app_err_t uart_bt_send_sensor_data(const sensor_data_t* data);

#endif // UART_BT_H
