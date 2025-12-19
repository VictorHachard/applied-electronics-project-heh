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

// Configuration du scheduler (en ticks de 10ms)
#define CONFIG_UPDATE_TICKS   10         // Recalcul config toutes les 10s (10 * 1s)

// Période d'acquisition dynamique (mise à jour depuis le datalogger)
static uint16_t g_sensor_period_ticks = 0;

// Flags externes de l'ISR
extern volatile uint8_t g_timer0_flag;
extern volatile uint8_t g_uart2_rx_flag;
extern volatile char g_uart2_rx_char;

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
    if (err == APP_EDEV) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("BMP280 Not Found");
        while(1);  // Bloquer l'exécution
    } else if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("BMP280 Init Err");
        while(1);  // Bloquer l'exécution
    }
    
    // Capteur SHT30 (humidité + température)
    err = sht30_init();
    if (err == APP_EDEV) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("SHT30 Not Found");
        while(1);  // Bloquer l'exécution
    } else if (err != APP_OK) {
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

/* ---------------------------------------------------------------------------
 * ÉTAPE 1 : CONFIGURATION DU DATALOGGER
 * -------------------------------------------------------------------------- */
void test_dl_setup(void)
{
    app_err_t err;
    char buf[17];

    // === Test 1.1 : Reset config ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.1 Reset config");
    __delay_ms(1000);

    err = dl_reset_config();
    if (err != APP_OK) {
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("OK");
    __delay_ms(1000);

    // === Test 1.2 : Vérifier qu'on ne peut pas démarrer sans période ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.2 Start w/o");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("period (must err)");
    __delay_ms(1000);

    rtc_time_t start = { 14, 15, 18, 12 };  // 14:15 le 18/12
    err = dl_set_running(&start);
    
    if (err == APP_ENOTCONFIG) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("Error detected:");
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String("APP_ENOTCONFIG");
        __delay_ms(1000);__delay_ms(1000);
        
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("Validation OK!");
        __delay_ms(1000);
    } else {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("FAIL: No error!");
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "Got: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }

    // === Test 1.3 : Configurer période de 1 minutes ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.3 Set period");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("5 minutes");
    __delay_ms(1000);

    err = dl_set_sample_period_min(1);
    if (err != APP_OK) {
        Lcd_Clear();
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("Period set: 1min");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("OK");
    __delay_ms(1000);

    // === Test 1.4 : Vérifier config ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.4 Check config");
    __delay_ms(1000);

    dl_cfg_t cfg;
    err = dl_get_config(&cfg);
    if (err != APP_OK) {
        sprintf(buf, "ERR: %d", err);
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String(buf);
        while(1);
    }

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    sprintf(buf, "P:%umin R:%d C:%d", cfg.sample_period_min, cfg.running, cfg.data_count);
    Lcd_Write_String(buf);
    __delay_ms(1000);__delay_ms(1000);

    // === Test 1.5 : Démarrer le datalogger ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.5 Start logger");
    __delay_ms(1000);

    err = dl_set_running(&start);
    if (err != APP_OK) {
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }

    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Running: YES");
    __delay_ms(1000);

    // === Test 1.6 : Vérif finale ===
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("1.6 Final check");
    __delay_ms(800);

    err = dl_get_config(&cfg);
    if (err != APP_OK || !cfg.running) {
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String("FAIL!");
        while(1);
    }

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("SETUP COMPLETE!");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Ready to log");
    __delay_ms(1000);__delay_ms(1000);
}

/* ---------------------------------------------------------------------------
 * ÉTAPE 2 : ENREGISTREMENT DE DONNÉES
 * -------------------------------------------------------------------------- */
void test_dl_log(void)
{
    app_err_t err;
    char buf[17];

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("2. LOG DATA TEST");
    __delay_ms(1000);

    // Données de test
    sensor_data_t samples[5] = {
        {2150, 6500, 101300},  // 21.50°C, 65.00%, 1013 hPa
        {2200, 6800, 101250},  // 22.00°C, 68.00%, 1012.5 hPa
        {2180, 6650, 101280},  // 21.80°C, 66.50%, 1012.8 hPa
        {2230, 7000, 101200},  // 22.30°C, 70.00%, 1012 hPa
        {2170, 6900, 101320},  // 21.70°C, 69.00%, 1013.2 hPa
    };

    for (uint8_t i = 0; i < 5; i++) {
        // Afficher données à enregistrer
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        sprintf(buf, "Record %d/5", i + 1);
        Lcd_Write_String(buf);

        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "T:%.1fC H:%.0f%%", 
                samples[i].t_c_x100 / 100.0,
                samples[i].rh_x100 / 100.0);
        Lcd_Write_String(buf);
        __delay_ms(1000);

        // Enregistrer
        err = dl_push_record(&samples[i]);
        
        if (err != APP_OK) {
            Lcd_Clear();
            Lcd_Set_Cursor(0, 0);
            if (err == APP_ENOTRUNNING) {
                Lcd_Write_String("Not running!");
            } else if (err == APP_EFULL) {
                Lcd_Write_String("Memory full!");
            } else {
                sprintf(buf, "Push ERR: %d", err);
                Lcd_Write_String(buf);
            }
            while(1);
        }

        // Confirmation
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        sprintf(buf, "Record %d: OK", i + 1);
        Lcd_Write_String(buf);
        
        // Afficher pression
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "P:%.1f hPa", samples[i].p_pa / 100.0);
        Lcd_Write_String(buf);
        __delay_ms(1200);
    }

    // Vérifier compteur
    uint8_t count = 0;
    dl_cfg_t cfg;
    err = dl_get_config(&cfg);
    if (err != APP_OK) {
        Lcd_Clear();
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }
    count = cfg.data_count;
    
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("Log complete!");
    Lcd_Set_Cursor(1, 0);
    sprintf(buf, "Total: %d rec", count);
    Lcd_Write_String(buf);
    __delay_ms(1000);__delay_ms(1000);
}

/* ---------------------------------------------------------------------------
 * ÉTAPE 3 : LECTURE ET VÉRIFICATION
 * -------------------------------------------------------------------------- */
void test_dl_read(void)
{
    app_err_t err;
    char buf[17];

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("3. READ DATA TEST");
    __delay_ms(1000);

    // Obtenir nombre d'enregistrements
    uint8_t count = 0;
    dl_cfg_t cfg;
    err = dl_get_config(&cfg);
    if (err != APP_OK) {
        Lcd_Clear();
        Lcd_Write_String("Count ERR!");
        while(1);
    }
    count = cfg.data_count;

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    sprintf(buf, "Records: %d", count);
    Lcd_Write_String(buf);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Reading...");
    __delay_ms(1000);

    // Lire et afficher chaque enregistrement
    for (uint8_t i = 0; i < count; i++) {
        sensor_data_t data;
        
        err = dl_read(i, &data);
        if (err != APP_OK) {
            Lcd_Clear();
            sprintf(buf, "Read %d ERR:%d", i, err);
            Lcd_Write_String(buf);
            __delay_ms(1000);__delay_ms(1000);
            continue;
        }

        // Page 1 : Température et humidité
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        sprintf(buf, "[%d] T: %.2fC", i, data.t_c_x100 / 100.0);
        Lcd_Write_String(buf);
        
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "RH: %.1f %%", data.rh_x100 / 100.0);
        Lcd_Write_String(buf);
        __delay_ms(1000);__delay_ms(1000);

        // Page 2 : Pression
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        sprintf(buf, "[%d] Pressure:", i);
        Lcd_Write_String(buf);
        
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "%.1f hPa", data.p_pa / 100.0);
        Lcd_Write_String(buf);
        __delay_ms(1000);__delay_ms(1000);
    }

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("READ COMPLETE!");
    Lcd_Set_Cursor(1, 0);
    sprintf(buf, "%d records OK", count);
    Lcd_Write_String(buf);
    __delay_ms(1000);__delay_ms(1000);
}

/* ---------------------------------------------------------------------------
 * ÉTAPE 4 : TESTS DE VALIDATION SUPPLÉMENTAIRES
 * -------------------------------------------------------------------------- */
void test_dl_validation(void)
{
    app_err_t err;
    char buf[17];

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("4. VALIDATION");
    __delay_ms(1000);

    // Test 4.1 : Arrêter le logger
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("4.1 Stop logger");
    __delay_ms(1000);

    err = dl_stop();
    if (err != APP_OK) {
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }

    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Stopped OK");
    __delay_ms(1000);

    // Test 4.2 : Vérifier qu'on ne peut plus logger
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("4.2 Try log");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("(must fail)");
    __delay_ms(1000);

    sensor_data_t dummy = {2500, 5000, 100000};
    err = dl_push_record(&dummy);

    if (err == APP_ENOTRUNNING) {
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("Error detected:");
        Lcd_Set_Cursor(1, 0);
        Lcd_Write_String("ENOTRUNNING");
        __delay_ms(1000);__delay_ms(1000);
        
        Lcd_Clear();
        Lcd_Set_Cursor(0, 0);
        Lcd_Write_String("Validation OK!");
        __delay_ms(1000);
    } else {
        Lcd_Clear();
        Lcd_Write_String("FAIL: logged!");
        while(1);
    }

    // Test 4.3 : Vérifier config finale
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("4.3 Final state");
    __delay_ms(1000);

    dl_cfg_t cfg;
    err = dl_get_config(&cfg);
    if (err != APP_OK) {
        Lcd_Set_Cursor(1, 0);
        sprintf(buf, "ERR: %d", err);
        Lcd_Write_String(buf);
        while(1);
    }

    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    sprintf(buf, "R:%d C:%d P:%umin", cfg.running, cfg.data_count, cfg.sample_period_min);
    Lcd_Write_String(buf);
    __delay_ms(1000);__delay_ms(1000);
}

/* ---------------------------------------------------------------------------
 * BOUCLE PRINCIPALE - TESTS
 * -------------------------------------------------------------------------- */
void app_main_loop_ok(void) {
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("DATALOGGER TESTS");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("Starting...");
    __delay_ms(1000);

    /* === PHASE 1 : CONFIGURATION === */
    test_dl_setup();

    /* === PHASE 2 : ENREGISTREMENT === */
    test_dl_log();

    /* === PHASE 3 : LECTURE === */
    test_dl_read();

    /* === PHASE 4 : VALIDATION === */
    test_dl_validation();

    /* === FIN DES TESTS === */
    Lcd_Clear();
    Lcd_Set_Cursor(0, 0);
    Lcd_Write_String("ALL TESTS PASS!");
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String("*** SUCCESS ***");

    // Boucle infinie
    while (1) {
        __delay_ms(1000);
    }
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
    dl_cfg_t current_cfg;
    app_err_t err;

    static uint16_t last_config_update_tick = 0;
    static uint16_t last_sensor_tick = 0;
    static bool period_locked = false;
    
    test_dl_setup();

    // Boucle infinie
    while (1) {
        // TÂCHE : Bluetooth RX
        if (g_uart2_rx_flag) {
            g_uart2_rx_flag = 0;
            bluetooth_handle_rx(g_uart2_rx_char);
        }

        // TÂCHE : Vérifier le tick du scheduler
        if (g_timer0_flag) {
            g_timer0_flag = 0;  // Effacer le flag
            g_tick_counter++;   // Incrémenter le compteur global
        }

        // TÂCHE PÉRIODIQUE : Mise à jour de la configuration
        if (!period_locked && (uint16_t)(g_tick_counter - last_config_update_tick) >= CONFIG_UPDATE_TICKS) {
            last_config_update_tick = g_tick_counter;

            err = dl_get_config(&current_cfg);
            if (err != APP_OK) {
                // gérer erreur
            } else {
                uint16_t period_s = (uint16_t)current_cfg.sample_period_min * 60u;
                if (period_s > 0u) {
                    g_sensor_period_ticks = period_s;
                    period_locked = true;
                }
                // Temporary thing
                g_sensor_period_ticks = 10;
            }
        }
        
        // TÂCHE PÉRIODIQUE : Acquisition des capteurs
        if (g_sensor_period_ticks > 0u &&
            (uint16_t)(g_tick_counter - last_sensor_tick) >= g_sensor_period_ticks) {

            last_sensor_tick = g_tick_counter;
            
                
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

                // Lcd_Clear();
                // Lcd_Set_Cursor(0, 0);
                // Lcd_Write_String("T:");
                // char buffer[16];
                // sprintf(buffer, "%.2fC ", sensor_data.t_c_x100 / 100.0);
                // Lcd_Write_String(buffer);
                // Lcd_Write_String("RH:");
                // sprintf(buffer, "%.2f%% ", sensor_data.rh_x100 / 100.0);
                // Lcd_Write_String(buffer);
                // Lcd_Set_Cursor(1, 0);
                // Lcd_Write_String("P:");
                // sprintf(buffer, "%.2fhPa", sensor_data.p_pa / 100.0);
                // Lcd_Write_String(buffer);

                err = dl_get_config(&current_cfg);
                if (err != APP_OK) {
                    // gérer erreur
                    continue;
                } else if (current_cfg.running) {
                    // Enregistrement dans le datalogger
                    err = dl_push_record(&sensor_data);
                    if (err == APP_EFULL) {
                        Lcd_Clear();
                        Lcd_Set_Cursor(0, 0);
                        Lcd_Write_String("DL Write Err");
                        Lcd_Set_Cursor(1, 0);
                        Lcd_Write_String("Memory Full");
                        while(1);  // Bloquer l'exécution
                    } else if (err != APP_OK){
                        Lcd_Clear();
                        Lcd_Set_Cursor(0, 0);
                        Lcd_Write_String("DL Push Err");
                        Lcd_Set_Cursor(1, 0);
                        Lcd_Write_String("Code:");
                        char buffer[16];
                        sprintf(buffer, "%d", err);
                        Lcd_Write_String(buffer);
                        while(1);  // Bloquer l'exécution
                    }
                    
                    // Use dl read to verify the written data
                    err = dl_get_config(&current_cfg);
                    if (err != APP_OK) {
                        // Erreur de lecture de la configuration
                        continue;  // Ignorer cette itération
                    }

                    sensor_data_t verify_data;
                    err = dl_read(current_cfg.data_count - 1, &verify_data);

                    if (err != APP_OK) {
                        Lcd_Clear();
                        Lcd_Set_Cursor(0, 0);
                        Lcd_Write_String("DL Read Err");
                        Lcd_Set_Cursor(1, 0);
                        Lcd_Write_String("Code:");
                        char buffer[16];
                        sprintf(buffer, "%d", err);
                        Lcd_Write_String(buffer);
                        while(1);  // Bloquer l'exécution
                    } else {
                        Lcd_Clear();
                        Lcd_Set_Cursor(0, 0);
                        char buffer[16];
                        sprintf(buffer, "Idx:%u", current_cfg.data_count - 1);
                        Lcd_Write_String(buffer);
                        Lcd_Set_Cursor(1, 0);
                        sprintf(buffer, "T:%.2fC", verify_data.t_c_x100 / 100.0);
                        Lcd_Write_String(buffer);
                    }
            }
        }
        
        // TÂCHE CONTINUE : Gestion du menu
        //app_loop();
    }
}
