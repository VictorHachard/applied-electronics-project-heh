// ===============================================
// File:   uart_pc.h
// Desc:   Header file for uart_pc.h
// ===============================================

#ifndef UART_PC_H
#define UART_PC_H

#include <stdint.h>
#include "core/types.h"

/*
 * UART 1 (RC6/RX1, RC7/TX1) pour communication PC via adaptateur USB-UART
 */
app_err_t uart_pc_init(uint32_t baud);

app_err_t uart_pc_send_sensor_data(const sensor_data_t* data);

#endif // UART_PC_H
