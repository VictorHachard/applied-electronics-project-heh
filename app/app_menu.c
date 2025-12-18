// ===============================================
// File:   app_menu.c
// Desc:   Source file for app_menu.c
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
    // Mettre à jour les boutons et le menu
    menu_update();
    
    // Rafraîchir l'affichage
    menu_display();
}
