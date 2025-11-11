// ===============================================
// File:   isr.c
// Desc:   Gestionnaire centralisé des interruptions
//         Point d'entrée unique pour toutes les ISR
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "isr.h"
#include <xc.h>
#include <stdint.h>

// ===============================================
// VARIABLES GLOBALES POUR LES INTERRUPTIONS
// ===============================================
volatile uint8_t g_timer0_flag = 0;   // Flag Timer0 (scheduler périodique)
volatile uint8_t g_uart1_rx_flag = 0; // Flag UART1 RX
volatile uint8_t g_uart2_rx_flag = 0; // Flag UART2 RX (Bluetooth)

// ===============================================
// FONCTION : isr_init
// ===============================================
// Initialise les interruptions globales et périphériques
//
//   - Timer0 : tick périodique (≈10 ms)
//   - UART1  : (à activer plus tard si besoin)
//   - UART2  : (à activer plus tard si besoin)
// ===============================================
void isr_init(void) {
    // Désactiver interruptions globales pendant la config
    INTCON0bits.GIE = 0;

    // --- Timer0 ---
    T0CON0bits.T0EN    = 0;      // OFF pendant la config
    T0CON0bits.T016BIT = 0;      // 8-bit
    T0CON1bits.T0CS    = 0b010;  // Fosc/4
    T0CON1bits.T0CKPS  = 0b1000; // Prescaler 1:256

    // Préchargement pour ~10 ms (approx) comme tu avais
    TMR0H = 256 - (625 / 256);
    TMR0L = 0;

    PIR3bits.TMR0IF = 0;   // Clear flag
    PIE3bits.TMR0IE = 1;   // Enable IT Timer0 (TMR0 est dans PIR3/PIE3)

    T0CON0bits.T0EN = 1;   // Start Timer0

    // Activer les interruptions globales
    INTCON0bits.GIE = 1;

    // ------------------------
    // Configuration UART (optionnel pour l’instant)
    // ------------------------
    // PIE3bits.U1RXIE = 1;   // à activer si tu veux l’IT RX UART1
    // PIE7bits.U2RXIE = 1;   // à activer si tu veux l’IT RX UART2
}

// ===============================================
// FONCTION : __interrupt() isr_handler
// ===============================================
// Point d'entrée unique pour toutes les interruptions
// ===============================================
void __interrupt() isr_handler(void) {
    // ------------------------
    // INTERRUPTION TIMER0
    // ------------------------
    if (PIR3bits.TMR0IF) {
        PIR3bits.TMR0IF = 0;   // Effacer le flag
        g_timer0_flag = 1;     // Indiquer au main loop qu’un tick est arrivé
    }

    // ------------------------
    // INTERRUPTION UART1 RX (à compléter)
    // ------------------------
    // if (PIR3bits.U1RXIF) {
    //     PIR3bits.U1RXIF = 0;
    //     g_uart1_rx_flag = 1;
    // }

    // ------------------------
    // INTERRUPTION UART2 RX (à compléter)
    // ------------------------
    // if (PIR7bits.U2RXIF) {
    //     PIR7bits.U2RXIF = 0;
    //     g_uart2_rx_flag = 1;
    // }
}