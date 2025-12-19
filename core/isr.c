// ===============================================
// File:   isr.c
// Desc:   Gestionnaire centralisé des interruptions
//         Point d'entrée unique pour toutes les ISR
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "isr.h"
#include <xc.h>
#include <stdint.h>

// Prototype de la callback boutons
extern void buttons_ioc_callback(void);
extern void buttons_update(void);

// Compteur pour menu (blink et scroll)
extern volatile uint16_t g_menu_tick_10ms;

// ===============================================
// VARIABLES GLOBALES POUR LES INTERRUPTIONS
// ===============================================
volatile uint8_t g_timer0_flag = 0;   // Flag Timer0 (scheduler périodique)
volatile uint8_t g_uart1_rx_flag = 0; // Flag UART1 RX
volatile uint8_t g_uart2_rx_flag = 0; // Flag UART2 RX (Bluetooth)
volatile char g_uart2_rx_char = 0;

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
    
    // --- Timer1 (10 millisecondes pour debounce) ---
    T1CONbits.TMR1ON = 0;      // OFF pendant la config
    T1CONbits.RD16 = 1;        // 16-bit mode
    T1CON = (T1CON & 0xCF) | 0x30; // Prescaler 1:8 (bits 5-4 = 11)
    T1CLK = 0b0001;            // Fosc/4
    
    // Préchargement pour 10ms avec Fosc = 64 MHz
    // Fosc/4 = 16 MHz, prescaler 1:8 = 2 MHz
    // Pour 10ms : 20000 cycles
    // Précharge = 65536 - 20000 = 45536 = 0xB1E0
    TMR1H = 0xB1;
    TMR1L = 0xE0;
    
    PIR4bits.TMR1IF = 0;   // Clear flag
    PIE4bits.TMR1IE = 1;   // Enable IT Timer1
    
    T1CONbits.TMR1ON = 1;  // Start Timer1

    // UART2 RX : Bluetooth
    PIR7bits.U2RXIF = 0;
    PIE7bits.U2RXIE = 1;

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
    // INTERRUPTION TIMER0 (1 seconde)
    // ------------------------
    if (PIR3bits.TMR0IF) {
        PIR3bits.TMR0IF = 0;   // Effacer le flag

        // Recharger pour la prochaine seconde
        TMR0H = 0x0B;
        TMR0L = 0xDC;

        g_timer0_flag = 1;     // Indiquer au main loop qu'un tick est arrivé
    }
    
    // ------------------------
    // INTERRUPTION TIMER1 (10 millisecondes pour debounce)
    // ------------------------
    if (PIR4bits.TMR1IF) {
        PIR4bits.TMR1IF = 0;   // Effacer le flag
        
        // Recharger pour les prochaines 10ms
        TMR1H = 0xB1;
        TMR1L = 0xE0;
        
        // Incrémenter le compteur global pour le menu (blink et scroll)
        g_menu_tick_10ms++;
        
        // Appeler la mise à jour des boutons pour le debounce
        buttons_update();
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
    if (PIR7bits.U2RXIF && PIE7bits.U2RXIE) {
        g_uart2_rx_char = U2RXB;
        g_uart2_rx_flag = 1;
    }
    
    // ------------------------
    // INTERRUPTION IOC (Boutons)
    // ------------------------
    if (PIR0bits.IOCIF) {
        // Effacer les flags IOC
        IOCAF = 0;
        IOCCF = 0;
        PIR0bits.IOCIF = 0;
        
        // Appeler le callback boutons
        buttons_ioc_callback();
    }
}