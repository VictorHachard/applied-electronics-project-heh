// ===============================================
// File:   app_menu.c
// Desc:   Application menu avec navigation
// ===============================================

#include "app_menu.h"
#include "buttons.h"
#include "menu.h"
#include "../drivers/lcd.h"

// ===============================================
// FONCTION : app_init
// ===============================================
// Initialise le système de menu et les boutons
// ===============================================
void app_init(void) {
    // Initialiser le système de boutons
    buttons_init();

    // Initialiser le menu
    menu_init();

    // Afficher le menu initial
    menu_display();
}

// ===============================================
// FONCTION : app_loop
// ===============================================
// Boucle de traitement du menu (à appeler régulièrement)
// ===============================================
void app_loop(void) {
    // Mettre à jour la logique du menu et vérifier si rafraîchissement nécessaire
    if (menu_update()) {
        // Rafraîchir l'affichage uniquement si un changement a eu lieu
        menu_display();
    }
}