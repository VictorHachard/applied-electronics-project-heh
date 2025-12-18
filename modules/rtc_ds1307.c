// ===============================================
// File:   rtc_ds1307.c
// Desc:   Source file for rtc_ds1307.c
// ===============================================

#include "rtc_ds1307.h"
#include "../drivers/i2c_bus.h"
#include <xc.h>

// ===============================================
// CONSTANTES PRIVÉES
// ===============================================
// Adresse I2C du DS1307 : 0x68 << 1 = 0xD0 (write) / 0xD1 (read)
#define RTC_ADDR_WRITE      0xD0
#define RTC_ADDR_READ       0xD1

// Bit CH (Clock Halt) dans le registre secondes
#define RTC_CH_BIT          0x80

// ===============================================
// FONCTIONS UTILITAIRES PRIVÉES
// ===============================================

// Conversion BCD → Décimal
static uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Conversion Décimal → BCD
static uint8_t dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

// ===============================================
// FONCTION : rtc_read_register
// ===============================================
// Lit un registre du DS1307
// ===============================================
static app_err_t rtc_read_register(uint8_t reg, uint8_t* value) {
    return i2c2_write_read(RTC_ADDR_WRITE, &reg, 1, RTC_ADDR_READ, value, 1);
}

// ===============================================
// FONCTION : rtc_write_register
// ===============================================
// Écrit dans un registre du DS1307
// ===============================================
static app_err_t rtc_write_register(uint8_t reg, uint8_t value) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    return i2c2_write(RTC_ADDR_WRITE, buf, 2);
}

// ===============================================
// FONCTION : rtc_init
// ===============================================
// Initialise le RTC DS1307 :
//  1. Vérifie la présence du composant
//  2. Démarre l'oscillateur (clear bit CH)
//  3. Désactive la sortie square wave
// ===============================================
app_err_t rtc_init(void) {
    app_err_t err;
    uint8_t seconds_reg;
    
    // Vérifier la présence du DS1307 en lisant le registre secondes
    err = rtc_read_register(RTC_REG_SECONDS, &seconds_reg);
    if (err != APP_OK) {
        return APP_EDEV;  // Composant non présent ou erreur I2C
    }
    
    // Si le bit CH (Clock Halt) est à 1, l'oscillateur est arrêté
    // Il faut le démarrer en mettant CH à 0
    if (seconds_reg & RTC_CH_BIT) {
        // Garder la valeur des secondes mais effacer le bit CH
        seconds_reg &= ~RTC_CH_BIT;
        err = rtc_write_register(RTC_REG_SECONDS, seconds_reg);
        if (err != APP_OK) {
            return err;
        }
    }
    
    // Désactiver la sortie square wave (optionnel)
    // Registre de contrôle : bit OUT=0, SQWE=0
    err = rtc_write_register(RTC_REG_CONTROL, 0x00);
    if (err != APP_OK) {
        return err;
    }
    
    return APP_OK;
}

// ===============================================
// FONCTION : rtc_set_time
// ===============================================
// Règle l'heure et la date du RTC
//  - Convertit les valeurs décimales en BCD
//  - Écrit dans les registres correspondants
//  - Assure que l'oscillateur reste actif (CH=0)
// ===============================================
app_err_t rtc_set_time(const rtc_time_t* time) {
    app_err_t err;
    
    if (time == NULL) {
        return APP_EPARAM;
    }
    
    // Validation basique des paramètres
    if (time->hour > 23 || time->min > 59 ||
        time->day == 0 || time->day > 31 ||
        time->month == 0 || time->month > 12) {
        return APP_EPARAM;
    }
    
    // Note: On pourrait écrire tous les registres en une seule transaction
    // pour plus d'efficacité, mais on fait simple ici
    
    // Secondes (on met 0 et on s'assure que CH=0)
    err = rtc_write_register(RTC_REG_SECONDS, 0x00);
    if (err != APP_OK) return err;
    
    // Minutes
    err = rtc_write_register(RTC_REG_MINUTES, dec_to_bcd(time->min));
    if (err != APP_OK) return err;
    
    // Heures (format 24h)
    err = rtc_write_register(RTC_REG_HOURS, dec_to_bcd(time->hour));
    if (err != APP_OK) return err;
    
    // Date du mois
    err = rtc_write_register(RTC_REG_DATE, dec_to_bcd(time->day));
    if (err != APP_OK) return err;
    
    // Mois
    err = rtc_write_register(RTC_REG_MONTH, dec_to_bcd(time->month));
    if (err != APP_OK) return err;
    
    return APP_OK;
}

// ===============================================
// FONCTION : rtc_get_time
// ===============================================
// Lit l'heure et la date depuis le RTC
//  - Lit les registres en séquence
//  - Convertit les valeurs BCD en décimal
//  - Remplit la structure rtc_time_t
// ===============================================
app_err_t rtc_get_time(rtc_time_t* time) {
    app_err_t err;
    uint8_t buf[7];  // Buffer pour lire les 7 premiers registres
    uint8_t reg_start = RTC_REG_SECONDS;
    
    if (time == NULL) {
        return APP_EPARAM;
    }
    
    // Lecture en burst des 7 registres (0x00 à 0x06)
    // Plus efficace que 7 lectures séparées
    err = i2c2_write_read(RTC_ADDR_WRITE, &reg_start, 1, RTC_ADDR_READ, buf, 7);
    if (err != APP_OK) {
        return err;
    }
    
    // Extraction et conversion BCD → Décimal
    // buf[0] = secondes (on ignore, pas dans rtc_time_t)
    time->min   = bcd_to_dec(buf[1] & 0x7F);      // Minutes
    time->hour  = bcd_to_dec(buf[2] & 0x3F);      // Heures (masque format 24h)
    // buf[3] = jour de la semaine (ignoré)
    time->day   = bcd_to_dec(buf[4] & 0x3F);      // Date
    time->month = bcd_to_dec(buf[5] & 0x1F);      // Mois
    // buf[6] = année (ignoré car pas dans rtc_time_t)
    
    return APP_OK;
}