// ===============================================
// File:   buttons.c
// Desc:   Gestion des boutons avec anti-rebond
// ===============================================

#include "buttons.h"
#include "../core/board.h"
#include <xc.h>

// ===============================================
// CONSTANTES
// ===============================================
#define DEBOUNCE_DELAY_MS   20    // Délai anti-rebond en ms
#define REPEAT_DELAY_MS     500   // Délai avant répétition auto
#define REPEAT_RATE_MS      100   // Taux de répétition auto

// ===============================================
// STRUCTURES PRIVÉES
// ===============================================
typedef struct {
    uint8_t current;           // État actuel du bouton (0=relâché, 1=pressé)
    uint8_t previous;          // État précédent
    uint8_t stable;            // État stable après debounce
    uint16_t debounce_counter; // Compteur pour anti-rebond
    uint16_t hold_counter;     // Compteur pour maintien
} button_info_t;

// ===============================================
// VARIABLES PRIVÉES
// ===============================================
static button_info_t buttons[BTN_COUNT];
static uint32_t last_update_tick = 0;

// ===============================================
// FONCTIONS PRIVÉES
// ===============================================

// Lit l'état brut d'un bouton (0=pressé car pull-up)
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
        buttons[i].current = 0;
        buttons[i].previous = 0;
        buttons[i].stable = 0;
        buttons[i].debounce_counter = 0;
        buttons[i].hold_counter = 0;
    }
    
    last_update_tick = 0;
}

void buttons_update(void) {
    uint8_t i;
    uint8_t raw_state;
    
    // Pour chaque bouton
    for (i = 0; i < BTN_COUNT; i++) {
        // Lire l'état brut
        raw_state = read_button_raw((button_t)i);
        
        // Si l'état a changé
        if (raw_state != buttons[i].current) {
            buttons[i].current = raw_state;
            buttons[i].debounce_counter = 0;
        } 
        // Si l'état est stable
        else {
            // Incrémenter le compteur de debounce
            if (buttons[i].debounce_counter < DEBOUNCE_DELAY_MS) {
                buttons[i].debounce_counter++;
                
                // Si le délai est écoulé, l'état est confirmé
                if (buttons[i].debounce_counter >= DEBOUNCE_DELAY_MS) {
                    buttons[i].previous = buttons[i].stable;
                    buttons[i].stable = buttons[i].current;
                    
                    // Reset du compteur de maintien si bouton relâché
                    if (buttons[i].stable == 0) {
                        buttons[i].hold_counter = 0;
                    }
                }
            }
            // Si bouton maintenu, incrémenter le compteur
            else if (buttons[i].stable == 1) {
                if (buttons[i].hold_counter < 0xFFFF) {
                    buttons[i].hold_counter++;
                }
            }
        }
    }
}

uint8_t button_pressed(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    
    // Détection du front montant (passage de 0 à 1)
    if (buttons[btn].stable == 1 && buttons[btn].previous == 0) {
        return 1;
    }
    
    return 0;
}

uint8_t button_held(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    
    return (buttons[btn].stable == 1 && 
            buttons[btn].hold_counter > (REPEAT_DELAY_MS / 10));
}

uint8_t button_get_raw(button_t btn) {
    if (btn >= BTN_COUNT) return 0;
    return read_button_raw(btn);
}
