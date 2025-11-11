// ===============================================
// File:   isr.h
// Desc:   Gestionnaire centralisé des interruptions
//         Point d'entrée unique pour toutes les ISR
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Initialisation des interruptions
void isr_init(void);

// Gestionnaire d'interruptions (point d'entrée unique)
void __interrupt() isr_handler(void);

// Flags volatiles accessibles depuis main
extern volatile uint8_t g_timer0_flag;   // Flag Timer0 (scheduler périodique)
extern volatile uint8_t g_uart1_rx_flag; // Flag UART1 RX
extern volatile uint8_t g_uart2_rx_flag; // Flag UART2 RX (Bluetooth)

#endif // ISR_H
