// ===============================================
// File:   menu.c
// Desc:   Système de menu navigable avec scroll
// ===============================================

#include "menu.h"
#include "buttons.h"
#include "../drivers/lcd.h"
#include "../modules/rtc_ds1307.h"
#include "../modules/bmp280.h"
#include "../modules/sht30.h"
#include "../modules/datalogger.h"
#include "../modules/eeprom_m93c66.h"
#include <stdio.h>
#include <string.h>

// ===============================================
// CONSTANTES PRIVÉES
// ===============================================
#define SCROLL_DELAY        100   // Délai entre chaque scroll (ms)
#define BLINK_DELAY         25   // Délai pour clignotement en édition

// Textes du menu principal
static const char* main_menu_items[] = {
    "1.Date & Heure",
    "2.Pression & Temp",
    "3.Humidite & Temp",
    "4.Datalogger Cfg",
    "5.Clear EEPROM"
};
#define MAIN_MENU_COUNT 5

// ===============================================
// VARIABLES PRIVÉES
// ===============================================
static menu_context_t ctx;
static rtc_time_t current_time;
static rtc_time_t edit_time;
static bmp280_data_t bmp_data;
static sht30_data_t sht_data;
static dl_cfg_t dl_config;
static uint8_t edit_delta_t = 1;  // Delta t en minutes (1-255)
static uint16_t blink_counter = 0;
static uint8_t blink_state = 0;
static uint8_t force_refresh = 0;  // Flag pour forcer le rafraîchissement

// ===============================================
// FONCTIONS UTILITAIRES PRIVÉES
// ===============================================

// Limite une valeur entre min et max
static uint8_t clamp(uint8_t value, uint8_t min, uint8_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Obtient le maximum pour un champ de date/heure
static uint8_t get_field_max(datetime_field_t field) {
    switch (field) {
        case FIELD_DAY:    return 31;
        case FIELD_MONTH:  return 12;
        case FIELD_YEAR:   return 99;
        case FIELD_HOUR:   return 23;
        case FIELD_MINUTE: return 59;
        case FIELD_SECOND: return 59;
        default:           return 0;
    }
}

// Obtient le minimum pour un champ de date/heure
static uint8_t get_field_min(datetime_field_t field) {
    switch (field) {
        case FIELD_DAY:
        case FIELD_MONTH:  return 1;
        default:           return 0;
    }
}

// Obtient/modifie une valeur d'un champ de date/heure
static uint8_t get_field_value(datetime_field_t field) {
    switch (field) {
        case FIELD_DAY:    return edit_time.day;
        case FIELD_MONTH:  return edit_time.month;
        case FIELD_YEAR:   return 25; // Placeholder
        case FIELD_HOUR:   return edit_time.hour;
        case FIELD_MINUTE: return edit_time.min;
        case FIELD_SECOND: return 0; // Placeholder
        default:           return 0;
    }
}

static void set_field_value(datetime_field_t field, uint8_t value) {
    switch (field) {
        case FIELD_DAY:    edit_time.day = value; break;
        case FIELD_MONTH:  edit_time.month = value; break;
        case FIELD_YEAR:   /* Placeholder */ break;
        case FIELD_HOUR:   edit_time.hour = value; break;
        case FIELD_MINUTE: edit_time.min = value; break;
        case FIELD_SECOND: /* Placeholder */ break;
    }
}

// ===============================================
// FONCTIONS D'AFFICHAGE
// ===============================================

// Affiche une ligne de menu avec numéro et scroll
static void display_menu_line(uint8_t row, const char* text, uint8_t selected, uint8_t scroll_offset) {
    char buffer[LCD_WIDTH + 1];
    char display_buf[LCD_WIDTH + 1];
    uint8_t prefix_len = 0;
    uint8_t text_start = 0;
    uint8_t i;
    
    Lcd_Set_Cursor(row, 0);
    
    // Afficher le curseur de sélection
    if (selected) {
        Lcd_Write_Char('>');
    } else {
        Lcd_Write_Char(' ');
    }
    
    // Trouver le préfixe numérique (ex: "1.")
    for (i = 0; text[i] != '\0' && text[i] != '.'; i++) {
        if (text[i] >= '0' && text[i] <= '9') {
            prefix_len = i + 2; // Inclut le point
        } else {
            break;
        }
    }
    
    // Si préfixe trouvé
    if (prefix_len > 0 && text[prefix_len - 1] == '.') {
        // Afficher le préfixe (fixe)
        for (i = 0; i < prefix_len; i++) {
            Lcd_Write_Char(text[i]);
        }
        text_start = prefix_len;
    }
    
    // Calculer l'espace disponible pour le texte
    uint8_t available_space = LCD_WIDTH - 1 - prefix_len; // -1 pour le curseur
    uint8_t text_len = strlen(&text[text_start]);
    
    // Appliquer le scroll si nécessaire
    if (text_len > available_space) {
        uint8_t max_offset = text_len - available_space;
        if (scroll_offset > max_offset) {
            scroll_offset = max_offset;
        }
        
        strncpy(display_buf, &text[text_start + scroll_offset], available_space);
        display_buf[available_space] = '\0';
    } else {
        strncpy(display_buf, &text[text_start], available_space);
        display_buf[text_len] = '\0';
    }
    
    // Afficher le texte
    Lcd_Write_String(display_buf);
    
    // Remplir le reste avec des espaces
    for (i = strlen(display_buf) + 1 + prefix_len; i < LCD_WIDTH; i++) {
        Lcd_Write_Char(' ');
    }
}

// Affiche le menu principal
static void display_main_menu(void) {
    uint8_t current_item = ctx.main_index;
    uint8_t next_item = (current_item + 1) % MAIN_MENU_COUNT;
    
    display_menu_line(0, main_menu_items[current_item], 1, ctx.scroll_offset);
    display_menu_line(1, main_menu_items[next_item], 0, 0);
}

// Affiche le sous-menu Date & Heure
static void display_datetime_submenu(void) {
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    // Lire l'heure actuelle
    rtc_get_time(&current_time);
    
    if (ctx.state == MENU_STATE_EDIT_SELECT || ctx.state == MENU_STATE_EDIT_VALUE) {
        // Mode édition - afficher avec curseur
        snprintf(line1, sizeof(line1), "Date:%02d/%02d/2025", 
                 edit_time.day, edit_time.month);
        snprintf(line2, sizeof(line2), "Heure:%02d:%02d:00", 
                 edit_time.hour, edit_time.min);
    } else {
        // Mode affichage normal
        const char* options[] = {"Date", "Heure"};
        display_menu_line(0, options[ctx.submenu_index], 1, 0);
        
        if (ctx.submenu_index == 0) {
            snprintf(line2, sizeof(line2), " %02d/%02d/2025", 
                     current_time.day, current_time.month);
        } else {
            snprintf(line2, sizeof(line2), " %02d:%02d:00", 
                     current_time.hour, current_time.min);
        }
        
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String(line2);
        return;
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
    
    // Afficher le curseur en mode édition
    if (ctx.state == MENU_STATE_EDIT_VALUE && blink_state) {
        uint8_t cursor_pos = 0;
        uint8_t cursor_row = 0;
        
        switch (ctx.edit_field) {
            case FIELD_DAY:    cursor_row = 0; cursor_pos = 5; break;
            case FIELD_MONTH:  cursor_row = 0; cursor_pos = 8; break;
            case FIELD_HOUR:   cursor_row = 1; cursor_pos = 6; break;
            case FIELD_MINUTE: cursor_row = 1; cursor_pos = 9; break;
            default: break;
        }
        
        Lcd_Set_Cursor(cursor_row, cursor_pos);
        Lcd_Write_String("__");
    }
}

// Affiche le indx et les valeur en indx - 1 du datalogger
static void display_datalogger_index(void) {
    dl_cfg_t cfg;
    app_err_t err;
    sensor_data_t data;
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    err = dl_get_config(&cfg);
    if (err != APP_OK) {
        snprintf(line1, sizeof(line1), "DL Config Err");
        snprintf(line2, sizeof(line2), "Err Code:%d", err);
    } else {
        snprintf(line1, sizeof(line1), "DL Idx:%d", cfg.data_count);
        err = dl_read(cfg.data_count - 1, (sensor_data_t*)&data);
        if (err != APP_OK) {
            snprintf(line2, sizeof(line2), "DL Read Err:%d", err);
        } else {
            int32_t temp = data.t_c_x100 / 100;
            int32_t temp_dec = data.t_c_x100 % 100;
            uint32_t rh = data.rh_x100 / 100;
            uint32_t rh_dec = data.rh_x100 % 100;
            snprintf(line2, sizeof(line2), "T:%ld.%02ldC RH:%lu.%02lu%%", 
                     temp, temp_dec, rh, rh_dec);
        }
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
}

// Affiche les données du BMP280
static void display_bmp280_data(void) {
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    if (bmp280_read(&bmp_data) == APP_OK) {
        // Conversion des données
        int32_t temp = bmp_data.temp_c_x100 / 100;
        int32_t temp_dec = bmp_data.temp_c_x100 % 100;
        int32_t press = bmp_data.pressure_pa / 100; // Pa -> hPa
        
        snprintf(line1, sizeof(line1), "Pres:%ld hPa", press);
        snprintf(line2, sizeof(line2), "Temp:%ld.%02ld C", temp, temp_dec);
    } else {
        snprintf(line1, sizeof(line1), "Pres: ERROR");
        snprintf(line2, sizeof(line2), "Temp: ERROR");
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
}

// Affiche les données du SHT30
static void display_sht30_data(void) {
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    if (sht30_read(&sht_data) == APP_OK) {
        // Conversion des données
        uint16_t rh = sht_data.rh_x100 / 100;
        uint16_t rh_dec = sht_data.rh_x100 % 100;
        int16_t temp = sht_data.temp_c_x100 / 100;
        int16_t temp_dec = sht_data.temp_c_x100 % 100;
        
        snprintf(line1, sizeof(line1), "RH:%d.%02d %%", rh, rh_dec);
        snprintf(line2, sizeof(line2), "Temp:%d.%02d C", temp, temp_dec);
    } else {
        snprintf(line1, sizeof(line1), "RH: ERROR");
        snprintf(line2, sizeof(line2), "Temp: ERROR");
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
}

// Affiche le sous-menu du datalogger
static void display_datalogger_submenu(void) {
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    // Lire la config actuelle
    dl_get_config(&dl_config);
    
    if (ctx.state == MENU_STATE_EDIT_VALUE) {
        // Mode édition
        if (ctx.dl_edit_field == FIELD_DL_DELTA_T) {
            snprintf(line1, sizeof(line1), "Delta t:%d min", edit_delta_t);
            snprintf(line2, sizeof(line2), "UP/DOWN modifier");
            
            // Clignotement
            if (blink_state) {
                Lcd_Set_Cursor(0, 8);
                Lcd_Write_String("___");
            }
        } else {
            // Champ Start/Stop
            const char* status = dl_config.running ? "RUNNING" : "STOPPED";
            snprintf(line1, sizeof(line1), "Status:%s", status);
            snprintf(line2, sizeof(line2), "ENTER:toggle");
        }
    } else {
        // Mode affichage normal
        const char* options[] = {"Delta t", "Start/Stop"};
        display_menu_line(0, options[ctx.submenu_index], 1, 0);
        
        if (ctx.submenu_index == 0) {
            snprintf(line2, sizeof(line2), " %d min", dl_config.sample_period_min);
        } else {
            const char* status = dl_config.running ? "RUNNING" : "STOPPED";
            snprintf(line2, sizeof(line2), " %s", status);
        }
        
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String(line2);
        return;
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
}

// Affiche la confirmation de clear EEPROM
static void display_clear_eeprom(void) {
    char line1[LCD_WIDTH + 1];
    char line2[LCD_WIDTH + 1];
    
    if (ctx.state == MENU_STATE_EDIT_VALUE) {
        // En cours d'effacement
        snprintf(line1, sizeof(line1), "Clearing...");
        snprintf(line2, sizeof(line2), "Please wait");
    } else {
        snprintf(line1, sizeof(line1), "Clear EEPROM?");
        snprintf(line2, sizeof(line2), "ENTER:Yes BACK:No");
    }
    
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String(line1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(line2);
}

// ===============================================
// GESTION DES ÉTATS
// ===============================================

// Gère le menu principal
static void handle_main_menu(void) {
    // Navigation
    if (button_pressed(BTN_DOWN)) {
        ctx.main_index = (ctx.main_index + 1) % MAIN_MENU_COUNT;
        ctx.scroll_offset = 0;
        ctx.scroll_counter = 0;
    }
    
    if (button_pressed(BTN_UP)) {
        if (ctx.main_index == 0) {
            ctx.main_index = MAIN_MENU_COUNT - 1;
        } else {
            ctx.main_index--;
        }
        ctx.scroll_offset = 0;
        ctx.scroll_counter = 0;
    }
    
    // Entrer dans le sous-menu
    if (button_pressed(BTN_ENTER)) {
        ctx.current_submenu = (submenu_type_t)ctx.main_index;
        ctx.submenu_index = 0;
        ctx.scroll_offset = 0;
        ctx.scroll_counter = 0;
        
        if (ctx.current_submenu == SUBMENU_DATETIME || 
            ctx.current_submenu == SUBMENU_DATALOGGER) {
            ctx.state = MENU_STATE_SUBMENU;
        } else {
            ctx.state = MENU_STATE_DISPLAY;
        }
    }
    
    // Gestion du scroll
    ctx.scroll_counter++;
    if (ctx.scroll_counter > SCROLL_DELAY) {
        ctx.scroll_counter = 0;
        
        const char* current_text = main_menu_items[ctx.main_index];
        uint8_t text_len = strlen(current_text);
        uint8_t prefix_len = 2; // "X."
        
        if (text_len - prefix_len > LCD_WIDTH - 1 - prefix_len) {
            ctx.scroll_offset++;
            if (ctx.scroll_offset > text_len - (LCD_WIDTH - 1)) {
                ctx.scroll_offset = 0;
            }
        }
    }
}

// Gère le sous-menu Date & Heure
static void handle_datetime_submenu(void) {
    // Navigation entre Date et Heure
    if (button_pressed(BTN_DOWN)) {
        ctx.submenu_index = (ctx.submenu_index + 1) % 2;
    }
    
    if (button_pressed(BTN_UP)) {
        ctx.submenu_index = (ctx.submenu_index == 0) ? 1 : 0;
    }
    
    // Entrer en mode édition
    if (button_pressed(BTN_ENTER)) {
        rtc_get_time(&edit_time);
        ctx.edit_field = (ctx.submenu_index == 0) ? FIELD_DAY : FIELD_HOUR;
        ctx.state = MENU_STATE_EDIT_VALUE;
        blink_counter = 0;
        blink_state = 1;
    }
    
    // Retour au menu principal
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_MAIN;
    }
}

// Gère le mode édition de date/heure
static void handle_datetime_edit(void) {
    uint8_t value = get_field_value(ctx.edit_field);
    uint8_t min_val = get_field_min(ctx.edit_field);
    uint8_t max_val = get_field_max(ctx.edit_field);
    
    // Incrémenter la valeur
    if (button_pressed(BTN_UP)) {
        value++;
        if (value > max_val) value = min_val;
        set_field_value(ctx.edit_field, value);
    }
    
    // Décrémenter la valeur
    if (button_pressed(BTN_DOWN)) {
        if (value <= min_val) {
            value = max_val;
        } else {
            value--;
        }
        set_field_value(ctx.edit_field, value);
    }
    
    // Passer au champ suivant
    if (button_pressed(BTN_ENTER)) {
        // Déterminer le champ suivant
        if (ctx.submenu_index == 0) { // Date
            if (ctx.edit_field == FIELD_DAY) {
                ctx.edit_field = FIELD_MONTH;
            } else {
                // Sauvegarder et sortir
                rtc_set_time(&edit_time);
                ctx.state = MENU_STATE_SUBMENU;
            }
        } else { // Heure
            if (ctx.edit_field == FIELD_HOUR) {
                ctx.edit_field = FIELD_MINUTE;
            } else {
                // Sauvegarder et sortir
                rtc_set_time(&edit_time);
                ctx.state = MENU_STATE_SUBMENU;
            }
        }
    }
    
    // Annuler l'édition
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_SUBMENU;
    }
    
    // Gestion du clignotement
    blink_counter++;
    if (blink_counter > BLINK_DELAY) {
        blink_counter = 0;
        blink_state = !blink_state;
    }
}

// Gère l'affichage des capteurs
static void handle_display_state(void) {
    // Forcer le rafraîchissement pour mettre à jour les données des capteurs
    force_refresh = 1;
    
    // Retour au menu principal
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_MAIN;
        force_refresh = 0;
    }
}

// Gère le sous-menu Datalogger
static void handle_datalogger_submenu(void) {
    // Navigation entre Delta t et Start/Stop
    if (button_pressed(BTN_DOWN)) {
        ctx.submenu_index = (ctx.submenu_index + 1) % 2;
    }
    
    if (button_pressed(BTN_UP)) {
        ctx.submenu_index = (ctx.submenu_index == 0) ? 1 : 0;
    }
    
    // Entrer en mode édition
    if (button_pressed(BTN_ENTER)) {
        dl_get_config(&dl_config);
        
        if (ctx.submenu_index == 0) {
            // Édition Delta t
            edit_delta_t = dl_config.sample_period_min;
            if (edit_delta_t == 0) edit_delta_t = 1; // Défaut
            ctx.dl_edit_field = FIELD_DL_DELTA_T;
        } else {
            // Toggle Start/Stop
            ctx.dl_edit_field = FIELD_DL_START_STOP;
        }
        
        ctx.state = MENU_STATE_EDIT_VALUE;
        blink_counter = 0;
        blink_state = 1;
    }
    
    // Retour au menu principal
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_MAIN;
    }
}

// Gère l'édition du datalogger
static void handle_datalogger_edit(void) {
    if (ctx.dl_edit_field == FIELD_DL_DELTA_T) {
        // Édition Delta t
        if (button_pressed(BTN_UP)) {
            edit_delta_t++;
            if (edit_delta_t > 255) edit_delta_t = 1;
        }
        
        if (button_pressed(BTN_DOWN)) {
            if (edit_delta_t <= 1) {
                edit_delta_t = 255;
            } else {
                edit_delta_t--;
            }
        }
        
        // Sauvegarder
        if (button_pressed(BTN_ENTER)) {
            dl_set_sample_period_min(edit_delta_t);
            ctx.state = MENU_STATE_SUBMENU;
        }
        
        // Gestion du clignotement
        blink_counter++;
        if (blink_counter > BLINK_DELAY) {
            blink_counter = 0;
            blink_state = !blink_state;
        }
    } else {
        // Toggle Start/Stop
        if (button_pressed(BTN_ENTER)) {
            dl_get_config(&dl_config);
            
            if (dl_config.running) {
                // Arrêter le datalogger
                dl_stop();
            } else {
                // Démarrer le datalogger
                rtc_get_time(&current_time);
                dl_set_running(&current_time);
            }
            
            ctx.state = MENU_STATE_SUBMENU;
        }
    }
    
    // Annuler
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_SUBMENU;
    }
}

// Gère le menu Clear EEPROM
static void handle_clear_eeprom(void) {
    // Forcer le rafraîchissement pour afficher correctement
    force_refresh = 1;
    
    // Annuler (disponible dans tous les états)
    if (button_pressed(BTN_BACK)) {
        ctx.state = MENU_STATE_MAIN;
        force_refresh = 0;
        return;
    }
    
    if (ctx.state == MENU_STATE_DISPLAY) {
        // Confirmation
        if (button_pressed(BTN_ENTER)) {
            // Lancer l'effacement
            ctx.state = MENU_STATE_EDIT_VALUE;
            
            // Réinitialiser la config du datalogger
            app_err_t err = dl_reset_config();
            if (err != APP_OK) {
                Lcd_Clear();
                Lcd_Set_Cursor(0, 0);
                Lcd_Write_String("DL Reset ERROR");
                Lcd_Set_Cursor(1, 0);
                char buffer[16];
                sprintf(buffer, "Err Code:%d", err);
                Lcd_Write_String(buffer);
                while(1);  // Bloquer l'exécution
            }
            
            ctx.state = MENU_STATE_MAIN;
            force_refresh = 0;
        }
    }
}

// ===============================================
// API PUBLIQUE
// ===============================================

void menu_init(void) {
    ctx.state = MENU_STATE_MAIN;
    ctx.main_index = 0;
    ctx.submenu_index = 0;
    ctx.scroll_offset = 0;
    ctx.scroll_counter = 0;
    ctx.editing = 0;
    ctx.edit_field = FIELD_DAY;
    ctx.edit_cursor = 0;
    
    Lcd_Clear();
}

uint8_t menu_update(void) {
    uint8_t old_state = ctx.state;
    uint8_t old_index = ctx.main_index;
    uint8_t old_submenu_index = ctx.submenu_index;
    uint8_t old_blink = blink_state;
    
    // Machine à états
    switch (ctx.state) {
        case MENU_STATE_MAIN:
            handle_main_menu();
            break;
            
        case MENU_STATE_SUBMENU:
            if (ctx.current_submenu == SUBMENU_DATETIME) {
                handle_datetime_submenu();
            } else if (ctx.current_submenu == SUBMENU_DATALOGGER) {
                handle_datalogger_submenu();
            }
            break;
            
        case MENU_STATE_DISPLAY:
            if (ctx.current_submenu == SUBMENU_CLEAR_EEPROM) {
                handle_clear_eeprom();
            } else {
                handle_display_state();
            }
            break;
            
        case MENU_STATE_EDIT_VALUE:
            if (ctx.current_submenu == SUBMENU_DATETIME) {
                handle_datetime_edit();
            } else if (ctx.current_submenu == SUBMENU_DATALOGGER) {
                handle_datalogger_edit();
            }
            break;
            
        default:
            ctx.state = MENU_STATE_MAIN;
            return 1;
            break;
    }
    
    // Retourner 1 si quelque chose a changé ou si rafraîchissement forcé
    uint8_t needs_refresh = (old_state != ctx.state || 
                             old_index != ctx.main_index || 
                             old_submenu_index != ctx.submenu_index ||
                             old_blink != blink_state ||
                             force_refresh);
    
    // Réinitialiser le flag après utilisation
    if (force_refresh && needs_refresh) {
        force_refresh = 0;
    }
    
    return needs_refresh;
}

void menu_display(void) {
    Lcd_Clear();
    
    switch (ctx.state) {
        case MENU_STATE_MAIN:
            display_main_menu();
            break;
            
        case MENU_STATE_SUBMENU:
            if (ctx.current_submenu == SUBMENU_DATETIME) {
                display_datetime_submenu();
            } else if (ctx.current_submenu == SUBMENU_DATALOGGER) {
                display_datalogger_submenu();
            }
            break;
            
        case MENU_STATE_DISPLAY:
            if (ctx.current_submenu == SUBMENU_BMP280) {
                display_datalogger_index();
            } else if (ctx.current_submenu == SUBMENU_SHT30) {
                display_sht30_data();
            } else if (ctx.current_submenu == SUBMENU_CLEAR_EEPROM) {
                display_clear_eeprom();
            }
            break;
            
        case MENU_STATE_EDIT_VALUE:
            if (ctx.current_submenu == SUBMENU_DATETIME) {
                display_datetime_submenu();
            } else if (ctx.current_submenu == SUBMENU_DATALOGGER) {
                display_datalogger_submenu();
            }
            break;
            
        default:
            break;
    }
}
