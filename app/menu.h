// ===============================================
// File:   menu.h
// Desc:   Système de menu navigable avec scroll
// ===============================================

#ifndef MENU_H
#define MENU_H

#include <stdint.h>

// ===============================================
// CONSTANTES
// ===============================================
#define LCD_WIDTH   16
#define LCD_HEIGHT  2

// ===============================================
// ÉNUMÉRATIONS
// ===============================================

// États de la machine à états du menu
typedef enum {
    MENU_STATE_MAIN,           // Menu principal
    MENU_STATE_SUBMENU,        // Dans un sous-menu
    MENU_STATE_DISPLAY,        // Affichage de données
    MENU_STATE_EDIT_SELECT,    // Sélection du champ à éditer
    MENU_STATE_EDIT_VALUE      // Édition d'une valeur
} menu_state_t;

// Type de sous-menu
typedef enum {
    SUBMENU_DATETIME = 0,      // Date & Heure
    SUBMENU_BMP280,            // Pression & Température
    SUBMENU_SHT30,             // Humidité & Température
    SUBMENU_DATALOGGER,        // Configuration Datalogger
    SUBMENU_CLEAR_EEPROM       // Clear EEPROM
} submenu_type_t;

// Champs éditables pour Date/Heure
typedef enum {
    FIELD_DAY = 0,
    FIELD_MONTH,
    FIELD_YEAR,
    FIELD_HOUR,
    FIELD_MINUTE,
    FIELD_SECOND,
    FIELD_COUNT
} datetime_field_t;

// Champs éditables pour Datalogger
typedef enum {
    FIELD_DL_DELTA_T = 0,      // Delta t entre mesures
    FIELD_DL_START_STOP,       // Start/Stop
    FIELD_DL_COUNT
} datalogger_field_t;

// ===============================================
// STRUCTURES
// ===============================================

// Contexte du menu
typedef struct {
    menu_state_t state;
    uint8_t main_index;         // Index dans le menu principal
    uint8_t submenu_index;      // Index dans le sous-menu actuel
    submenu_type_t current_submenu;
    uint8_t scroll_offset;      // Offset pour le scroll horizontal
    uint16_t scroll_counter;    // Compteur pour temporisation du scroll
    uint8_t editing;            // Mode édition activé
    datetime_field_t edit_field;// Champ en cours d'édition
    datalogger_field_t dl_edit_field; // Champ datalogger en cours
    uint8_t edit_cursor;        // Position du curseur en édition
} menu_context_t;

// ===============================================
// API PUBLIQUE
// ===============================================

// Initialisation du menu
void menu_init(void);

// Mise à jour du menu (retourne 1 si rafraîchissement nécessaire)
uint8_t menu_update(void);

// Rafraîchissement de l'affichage
void menu_display(void);

#endif // MENU_H
