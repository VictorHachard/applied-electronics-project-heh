// ===============================================
// File:   buttons.c
// Desc:   Gestion des boutons avec anti-rebond par interruptions IOC
// ===============================================

#include "buttons.h"
#include "../core/board.h"
#include <xc.h>

// ===============================================
// CONSTANTES
// ===============================================
#define DEBOUNCE_DELAY_MS   50    // Délai anti-rebond en ms (via Timer0)
#define REPEAT_DELAY_MS     500   // Délai avant répétition auto en ms
#define REPEAT_RATE_MS      100   // Taux de répétition auto en ms

// ===============================================
// STRUCTURES PRIVÉES
// ===============================================
typedef struct {
    uint8_t pressed_flag;      // Flag d'appui détecté (front montant)
    uint8_t state;             // État actuel (0=relâché, 1=pressé)
    uint16_t debounce_timer;   // Timer anti-rebond
    uint16_t hold_timer;       // Timer de maintien
    uint8_t debouncing;        // En cours d'anti-rebond
} button_info_t;

// ===============================================
// VARIABLES PRIVÉES
// ===============================================
static volatile button_info_t buttons[BTN_COUNT];

// ===============================================
// FONCTIONS PRIVÉES
// ===============================================

// Lit l'état brut d'un bouton (1=pressé car inversé)
static uint8_t read_button_raw(button_t btn) {
    switch (btn) {
        case BTN_UP:    return !PORTAbits.RA7;  // Inversé car pull-up
        case BTN_DOWN:  return !PORTAbits.RA6;
        case BTN_ENTER: return !PORTCbits.RC0;
        case BTN_BACK:  return !PORTCbits.RC1;
        default:        return 0;
    }
}

// ===============================================
// FONCTIONS PUBLIQUES
// ===============================================

void buttons_init(void) {
    uint8_t i;
    
    // Initialiser toutes les structures de boutons
    for (i = 0; i < BTN_COUNT; i++) {
        buttons[i].pressed_flag = 0;
        buttons[i].state = 0;
        buttons[i].debounce_timer = 0;
        buttons[i].hold_timer = 0;
        buttons[i].debouncing = 0;
    }
    
    // Configuration des interruptions IOC (Interrupt-on-Change)
    // Pour PORTA (RA6, RA7)
    IOCAPbits.IOCAP6 = 1;  // Détection front montant RA6 (BTN_UP)
    IOCAPbits.IOCAP7 = 1;  // Détection front montant RA7 (BTN_DOWN)
    IOCANbits.IOCAN6 = 1;  // Détection front descendant RA6
    IOCANbits.IOCAN7 = 1;  // Détection front descendant RA7
    
    // Pour PORTC (RC0, RC1)
    IOCCPbits.IOCCP0 = 1;  // Détection front montant RC0 (BTN_ENTER)
    IOCCPbits.IOCCP1 = 1;  // Détection front montant RC1 (BTN_BACK)
    IOCCNbits.IOCCN0 = 1;  // Détection front descendant RC0
    IOCCNbits.IOCCN1 = 1;  // Détection front descendant RC1
    
    // Effacer les flags IOC
    IOCAF = 0;
    IOCCF = 0;
    
    // Activer les interruptions IOC
    PIR0bits.IOCIF = 0;    // Effacer le flag global IOC
    PIE0bits.IOCIE = 1;    // Activer l'interruption IOC
}

void buttons_update(void) {
    uint8_t i;
    
    // Traiter l'anti-rebond et la répétition (appelé toutes les 10ms depuis app_loop)
    for (i = 0; i < BTN_COUNT; i++) {
        // Timer anti-rebond
        if (buttons[i].debouncing && buttons[i].debounce_timer > 0) {
            buttons[i].debounce_timer--;
            
            if (buttons[i].debounce_timer == 0) {
                // Anti-rebond terminé, vérifier l'état final
                uint8_t raw = read_button_raw((button_t)i);
                
                if (raw == 1 && buttons[i].state == 0) {
                    // Front montant confirmé
                    buttons[i].state = 1;
                    buttons[i].pressed_flag = 1;
                    buttons[i].hold_timer = 0;
                } else if (raw == 0 && buttons[i].state == 1) {
                    // Front descendant confirmé
                    buttons[i].state = 0;
                    buttons[i].hold_timer = 0;
                }
                
                buttons[i].debouncing = 0;
            }
        }
        
        // Timer de maintien pour répétition automatique
        if (buttons[i].state == 1 && !buttons[i].debouncing) {
            buttons[i].hold_timer++;
            
            // Répétition automatique
            if (buttons[i].hold_timer > (REPEAT_DELAY_MS / 10)) {
                if ((buttons[i].hold_timer - (REPEAT_DELAY_MS / 10)) % (REPEAT_RATE_MS / 10) == 0) {
                    buttons[i].pressed_flag = 1;
                }
            }
        }
    }
}

uint8_t button_pressed(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    
    // Vérifier et consommer le flag
    if (buttons[btn].pressed_flag) {
        buttons[btn].pressed_flag = 0;
        return 1;
    }
    
    return 0;
}

uint8_t button_held(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    
    return (buttons[btn].state == 1 && 
            buttons[btn].hold_timer > (REPEAT_DELAY_MS / 10));
}

uint8_t button_get_raw(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    return read_button_raw(btn);
}

// ===============================================
// CALLBACK ISR (appelé depuis isr.c)
// ===============================================
void buttons_ioc_callback(void) {
    uint8_t i;
    
    // Démarrer l'anti-rebond pour tous les boutons qui ont changé
    for (i = 0; i < BTN_COUNT; i++) {
        if (!buttons[i].debouncing) {
            buttons[i].debouncing = 1;
            buttons[i].debounce_timer = DEBOUNCE_DELAY_MS / 10;  // En unités de 10ms
        }
    }
}
