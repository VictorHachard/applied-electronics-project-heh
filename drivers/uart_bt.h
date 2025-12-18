// ===============================================
// File:   uart_bt.h
// Desc:   Header file for uart_bt.h
// ===============================================

#ifndef UART_BT_H
#define UART_BT_H

#include <stdint.h>
#include "../core/types.h"

// Initialise UART2 (pins, baud rate)
app_err_t uart_bt_init(uint32_t baud);

// Envoie un caractère
void uart_bt_putc(char c);

// Envoie une chaîne
void uart_bt_puts(const char* s);

#endif // UART_BT_H
