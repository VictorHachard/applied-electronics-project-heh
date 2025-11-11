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
// Flags volatiles pour communication entre ISR et main loop
volatile uint8_t g_timer0_flag = 0;   // Flag Timer0 (scheduler périodique)
volatile uint8_t g_uart1_rx_flag = 0; // Flag réception UART1
volatile uint8_t g_uart2_rx_flag = 0; // Flag réception UART2 (Bluetooth)

// ===============================================
// FONCTION : isr_init
// ===============================================
// Initialise les interruptions globales et périphériques
//
// INTERRUPTIONS CONFIGURÉES :
//   - Timer0 : tick périodique (10ms)
//   - UART1  : réception de données
//   - UART2  : réception Bluetooth
//
// ===============================================
void isr_init(void) {
    // Désactiver temporairement les interruptions
    INTCONbits.GIE = 0;

    // Configuration Timer0 (interruption périodique)
    
    // Timer0 en mode 8-bit, prescaler 1:256
    T0CON0bits.T0EN = 0;      // Désactiver Timer0 pendant la config
    T0CON0bits.T016BIT = 0;   // Mode 8-bit
    T0CON1bits.T0CS = 0b010;  // Clock source = Fosc/4
    T0CON1bits.T0CKPS = 0b1000; // Prescaler 1:256
    
    // Calcul pour 10ms @ 64MHz : (64MHz/4)/256 = 62.5kHz
    // Pour 10ms : 62500Hz * 0.01s = 625 ticks
    // Comme Timer0 8-bit compte jusqu'à 256, il faut recharger
    TMR0H = 256 - 625/256;
    TMR0L = 0;
    
    // Activer l'interruption Timer0
    PIE0bits.TMR0IE = 1;      // Timer0 Interrupt Enable
    T0CON0bits.T0EN = 1;      // Démarrer Timer0
    
    // Configuration UART1
    // PIE3bits.U1RXIE = 1;
    
    // Configuration UART2
    // PIE3bits.U2RXIE = 1;
    
    // Activer les interruptions périphériques
    INTCONbits.PEIE = 1;
    
    // Activer les interruptions globales
    INTCONbits.GIE = 1;
}

// ===============================================
// FONCTION : __interrupt() isr_handler
// ===============================================
// Point d'entrée unique pour TOUTES les interruptions
//
// ===============================================
void __interrupt() isr_handler(void) {
    // INTERRUPTION TIMER0
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;  // Effacer le flag
        g_timer0_flag = 1;  // Lever le flag pour le main loop
    }
    
    // INTERRUPTION UART1 RX
    
    // INTERRUPTION UART2 RX
    
}
