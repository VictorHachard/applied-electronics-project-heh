// ===============================================
// File:   rtc_ds1307.h
// Desc:   Header file for rtc_ds1307.h
// ===============================================

#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include <stdint.h>
#include "../core/types.h"

// ===============================================
// REGISTRES DS1307
// ===============================================
#define RTC_REG_SECONDS     0x00  // Secondes (00-59)
#define RTC_REG_MINUTES     0x01  // Minutes (00-59)
#define RTC_REG_HOURS       0x02  // Heures (00-23)
#define RTC_REG_DAY         0x03  // Jour de la semaine (1-7)
#define RTC_REG_DATE        0x04  // Date du mois (01-31)
#define RTC_REG_MONTH       0x05  // Mois (01-12)
#define RTC_REG_YEAR        0x06  // Année (00-99)
#define RTC_REG_CONTROL     0x07  // Registre de contrôle

// ===============================================
// API PUBLIQUE
// ===============================================

// Initialise le module RTC (vérifie présence, démarre l'oscillateur)
app_err_t rtc_init(void);

// Règle l'heure et la date du RTC
app_err_t rtc_set_time(const rtc_time_t* time);

// Lit l'heure et la date depuis le RTC
app_err_t rtc_get_time(rtc_time_t* time);

#endif // RTC_DS1307_H
