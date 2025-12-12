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
//   - Timer0 : tick périodique (1s)
//   - UART1  : (à activer plus tard si besoin)
//   - UART2  : (à activer plus tard si besoin)
// ===============================================
void isr_init(void) {
    // Désactiver interruptions globales pendant la config
    INTCON0bits.GIE = 0;

    // --- Timer0 ---
    T0CON0bits.T0EN    = 0;      // OFF pendant la config
    T0CON0bits.T016BIT = 1;      // 16-bit MODE
    T0CON1bits.T0CS    = 0b010;  // Fosc/4
    T0CON1bits.T0ASYNC = 1;      // Synchrone
    T0CON1bits.T0CKPS  = 0b1000; // Prescaler 1:256

    // Préchargement pour 1 seconde avec Fosc = 64 MHz
    // Fosc/4 = 16 MHz, prescaler 1:256 = 62500 Hz
    // Pour 1s : 62500 cycles
    // Précharge = 65536 - 62500 = 3036 = 0x0BDC
    TMR0H = 0x0B;
    TMR0L = 0xDC;

    PIR3bits.TMR0IF = 0;   // Clear flag
    PIE3bits.TMR0IE = 1;   // Enable IT Timer0

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

        // Recharger pour la prochaine seconde
        TMR0H = 0x0B;
        TMR0L = 0xDC;

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