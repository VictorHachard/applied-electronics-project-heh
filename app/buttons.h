// ===============================================
// File:   buttons.h
// Desc:   Gestion des boutons avec anti-rebond
// ===============================================

#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

// Énumération des boutons
typedef enum {
    BTN_UP = 0,
    BTN_DOWN,
    BTN_ENTER,
    BTN_BACK,
    BTN_COUNT
} button_t;

// État d'un bouton
typedef enum {
    BTN_STATE_RELEASED = 0,
    BTN_STATE_PRESSED,
    BTN_STATE_HELD
} button_state_t;

// Initialisation du module de gestion des boutons
void buttons_init(void);

// Mise à jour de l'état des boutons (à appeler régulièrement)
void buttons_update(void);

// Vérifie si un bouton a été pressé (front montant)
uint8_t button_pressed(button_t btn);

// Vérifie si un bouton est maintenu
uint8_t button_held(button_t btn);

// Obtient l'état brut d'un bouton
uint8_t button_get_raw(button_t btn);

#endif // BUTTONS_H
