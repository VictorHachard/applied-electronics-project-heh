// ===============================================
// File:   uart_bt.c
// Desc:   Source file for uart_bt.c
// ===============================================

#include "uart_bt.h"
#include <xc.h>
#include "../core/board.h"


// ===============================================
// INITIALISATION UART2
// ===============================================

app_err_t uart_bt_init(uint32_t baud) {
    // Configuration pins UART2: TX=RB6, RX=RB7
    TRISBbits.TRISB6 = 0;       // TX = sortie
    TRISBbits.TRISB7 = 1;       // RX = entrée
    ANSELBbits.ANSELB6 = 0;     // Digital
    ANSELBbits.ANSELB7 = 0;     // Digital

    // PPS (Peripheral Pin Select)
    U2RXPPS = 0b00001111;       // RB7 → U2RX
    RB6PPS  = 0b00010110;       // U2TX → RB6

    // Calcul du baud rate
    // BRG = Fosc / (4 * baud) - 1
    uint16_t brg = (uint16_t)((_XTAL_FREQ / (4UL * baud)) - 1);
    U2BRG = brg;

    // Configuration UART2
    U2CON0 = 0b10110000;        // TXEN=1, RXEN=1, Async 8-bit
    U2CON1 = 0b10000000;        // ON=1
    U2CON2 = 0x00;

    return APP_OK;
}

// ===============================================
// ENVOI DE DONNÉES
// ===============================================

void uart_bt_putc(char c) {
    while (U2FIFObits.TXBE == 0);   // Attendre buffer vide
    U2TXB = c;
}

void uart_bt_puts(const char* s) {
    while (*s) {
        uart_bt_putc(*s++);
    }
}