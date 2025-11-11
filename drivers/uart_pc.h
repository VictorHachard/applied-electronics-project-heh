// ===============================================
// File:   uart_pc.h
// Desc:   Header file for uart_pc.h
// ===============================================

#ifndef UART_PC_H
#define UART_PC_H

#include <stdint.h>
#include "../core/types.h"

// UART 1 (RC6/TX1, RC7/RX1) pour communication PC via USB-UART
app_err_t uart_pc_init(uint32_t baud);

// Envoi des données capteurs via UART1
app_err_t uart_pc_send_sensor_data(const sensor_data_t* data);

#endif // UART_PC_H
