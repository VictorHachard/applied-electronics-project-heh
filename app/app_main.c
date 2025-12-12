// ===============================================
// File:   app_main.c
// Desc:   Boucle principale et scheduler de l'application
//         Intégration de tous les modules
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "app_main.h"
#include "../core/board.h"
//#include "../core/board.c"
#include "../core/isr.h"
//#include "../core/isr.c"
#include "../core/types.h"

// Modules drivers
#include "../drivers/i2c_bus.h"
//#include "../drivers/i2c_bus.c"
#include "../drivers/spi_bus.h"
//#include "../drivers/spi_bus.c"
#include "../drivers/uart_pc.h"
//#include "../drivers/uart_pc.c"
#include "../drivers/uart_bt.h"
//#include "../drivers/uart_bt.c"
#include "../drivers/lcd.h"

// Modules périphériques
#include "../modules/rtc_ds1307.h"
//#include "../modules/rtc_ds1307.c"
#include "../modules/bmp280.h"
//#include "../modules/bmp280.c"
#include "../modules/sht30.h"
//#include "../modules/sht30.c"
#include "../modules/eeprom_m93c66.h"
//#include "../modules/eeprom_m93c66.c"
#include "../modules/datalogger.h"
//#include "../modules/datalogger.c"
#include "../modules/bluetooth_proto.h"
//#include "../modules/bluetooth_proto.c"

// Application menu
#include "app_menu.h"
//#include "app_menu.c"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// ===============================================
// VARIABLES GLOBALES DU SCHEDULER
// ===============================================
// Compteurs pour les tâches périodiques
static uint16_t g_tick_counter = 0;      // Compteur de ticks (incrémenté par Timer0)
static uint16_t g_sensor_counter = 0;    // Compteur pour acquisition capteurs

// Configuration du scheduler (en ticks de 10ms)
#define CONFIG_UPDATE_TICKS   10         // Recalcul config toutes les 10s (10 * 1s)

// Période d'acquisition dynamique (mise à jour depuis le datalogger)
static uint16_t g_sensor_period_ticks = 0;

// Flags externes de l'ISR
extern volatile uint8_t g_timer0_flag;

// ===============================================
// FONCTION : app_main_init
// ===============================================
// Initialisation complète de l'application
//
// ORDRE D'INITIALISATION :
//   1. Matériel (board)
//   2. Initialisation de l'affichage (LCD)
//   3. Bus de communication (I2C, SPI, UART)
//   4. Périphériques (RTC, capteurs, EEPROM)
//   5. Modules applicatifs (datalogger, Bluetooth)
//   6. Interface utilisateur (LCD, menu)
//   7. Interruptions
//
// GESTION DES ERREURS :
//   - Si un module échoue, afficher un message d'erreur
//
// ===============================================
void app_main_init(void) {
    app_err_t err;
    
    // ÉTAPE 1 : Initialisation du matériel
    board_init();  // Oscillateur, TRIS, PPS

    // ÉTAPE 2 : Initialisation de l'affichage
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("Weather Station");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Initializing...");
    
    // ÉTAPE 3 : Initialisation des bus de communication
    // I2C pour RTC, SHT30, BMP280
    err = i2c_bus_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("I2C Init Error");
        while(1);  // Bloquer l'exécution
    }
    
    // SPI pour EEPROM M93C66
    err = spi_bus_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("SPI Init Error");
        while(1);  // Bloquer l'exécution
    }
    
    // UART1 pour communication PC (9600 bauds)
    err = uart_pc_init(9600);
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("UART1 Init Err");
        while(1);  // Bloquer l'exécution
    }
    
    // UART2 pour Bluetooth HC-05 (9600 bauds par défaut)
    err = uart_bt_init(9600);
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("UART2 Init Err");
        while(1);  // Bloquer l'exécution
    }
    
    // ÉTAPE 4 : Initialisation des périphériques
    // RTC DS1307 (horloge temps réel)
    err = rtc_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("RTC Init Error");
        while(1);  // Bloquer l'exécution
    }
    
    // Capteur BMP280 (pression + température)
    err = bmp280_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("BMP280 Init Err");
        while(1);  // Bloquer l'exécution
    }
    
    // Capteur SHT30 (humidité + température)
    err = sht30_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("SHT30 Init Err");
        while(1);  // Bloquer l'exécution
    }
    
    // EEPROM M93C66 (stockage datalogger)
    eeprom_init();
    
    // ÉTAPE 5 : Initialisation des modules applicatifs
    err = dl_reset_config();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("DL Config Err");
        while(1);  // Bloquer l'exécution
    }
    
    // ÉTAPE 6 : Initialisation de l'interface utilisateur
    err = bluetooth_init();
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("BT Init Error");
        while(1);  // Bloquer l'exécution
    }

    // Menu (gestion des écrans et navigation)
    app_init();
    
    // ÉTAPE 7 : Activation des interruptions
    isr_init();
    
    // Fin de l'initialisation
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("Ready!");
    __delay_ms(500);
}

// ===============================================
// FONCTION : app_main_loop
// ===============================================
// Boucle principale infinie de l'application
//
// TÂCHES PÉRIODIQUES :
//   1. Acquisition des capteurs
//   2. Enregistrement datalogger (selon configuration)
//   3. Gestion du menu (en continu)
//   4. Traitement des commandes Bluetooth (en continu)
//
// ===============================================
void app_main_loop(void) {
    sensor_data_t sensor_data;  // Données capteurs
    app_err_t err;

    static uint16_t last_config_update_tick = 0;
    static uint16_t last_sensor_read_tick = 0;
    
    // Boucle infinie
    while (1) {
        // TÂCHE : Vérifier le tick du scheduler
        if (g_timer0_flag) {
            g_timer0_flag = 0;  // Effacer le flag
            g_tick_counter++;   // Incrémenter le compteur global
            // Lcd_Clear();
            // Lcd_Set_Cursor(0, 0);
            // char buffer[16];
            // sprintf(buffer, "Tick: %u", g_tick_counter);
            // Lcd_Write_String(buffer);
        }

        // TÂCHE PÉRIODIQUE : Mise à jour de la configuration
        if (g_tick_counter % CONFIG_UPDATE_TICKS == 0 &&
            g_tick_counter != last_config_update_tick) {
            last_config_update_tick = g_tick_counter;
            dl_cfg_t current_cfg;
            err = dl_get_config(&current_cfg);
            if (err != APP_OK) {
                // Erreur de lecture de la configuration
                continue;  // Ignorer cette itération
            }
            uint8_t period_s = current_cfg.sample_period_s;
            // Lcd_Clear();
            // Lcd_Set_Cursor(0, 0);
            // char buffer[16];
            // sprintf(buffer, "Period: %us", period_s);
            // Lcd_Write_String(buffer);

            period_s = 10;

            // Convertir en ticks de 1s
            if (period_s > 0) {
                g_sensor_period_ticks = period_s;  // 1 tick = 1s
            } else {
                // Période invalide, désactiver l'acquisition
                g_sensor_period_ticks = 0;
            }
        }
        
        // TÂCHE PÉRIODIQUE : Acquisition des capteurs
        if (g_sensor_period_ticks > 0 &&
            g_tick_counter != last_sensor_read_tick) {
            last_sensor_read_tick = g_tick_counter;
            g_sensor_counter++;
            
            if (g_sensor_counter >= g_sensor_period_ticks) {
                g_sensor_counter = 0;  // Reset du compteur
                
                // Lire les capteurs BMP280 et SHT30
                bmp280_data_t bmp_data;
                sht30_data_t sht_data;
            
                err = bmp280_read(&bmp_data);
                if (err == APP_OK) {
                    sensor_data.p_pa = bmp_data.pressure_pa;
                    sensor_data.t_c_x100 = bmp_data.temp_c_x100;
                } else {
                    Lcd_Clear();
                    Lcd_Set_Cursor(0, 0);
                    Lcd_Write_String("BMP Read Err");
                    while(1);  // Bloquer l'exécution
                }

                err = sht30_read(&sht_data);
                if (err == APP_OK) {
                    sensor_data.rh_x100 = sht_data.rh_x100;
                } else {
                    Lcd_Clear();
                    Lcd_Set_Cursor(0, 0);
                    Lcd_Write_String("SHT Read Err");
                    while(1);  // Bloquer l'exécution
                }

                // print on lcd the sensor data
                Lcd_Clear();
                Lcd_Set_Cursor(0, 0);
                Lcd_Write_String("T:");
                char buffer[16];
                sprintf(buffer, "%.2fC ", sensor_data.t_c_x100 / 100.0);
                Lcd_Write_String(buffer);
                Lcd_Write_String("RH:");
                sprintf(buffer, "%.2f%% ", sensor_data.rh_x100 / 100.0);
                Lcd_Write_String(buffer);
                Lcd_Set_Cursor(1, 0);
                Lcd_Write_String("P:");
                sprintf(buffer, "%.2fPa", sensor_data.p_pa / 100.0);
                Lcd_Write_String(buffer);
                
                // // Enregistrement dans le datalogger
                // dl_cfg_t current_cfg;
                // err = dl_get_config(&current_cfg);
                // if (err != APP_OK) {
                //     // Erreur de lecture de la configuration
                //     continue;  // Ignorer cette itération
                // }
                // if (current_cfg.running) {
                //     err = dl_push_record(&sensor_data);
                //     if (err == APP_EFULL) {
                //         Lcd_Clear();
                //         Lcd_Set_Cursor(0, 0);
                //         Lcd_Write_String("DL Write Err");
                //         while(1);  // Bloquer l'exécution
                //     }
                // }
                
                // bluetooth_send_sensor_data(&sensor_data);
            }
        }
        
        // TÂCHE CONTINUE : Gestion du menu
        app_loop();
    }
}
