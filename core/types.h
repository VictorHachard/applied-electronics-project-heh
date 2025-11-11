// ===============================================
// File:   types.h
// Desc:   Structures de données et codes d'erreur
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/* STRUCTURE DE DONNÉES : rtc_time_t
 * ------------------------------------------------------------
 * Représentation du temps réel (date + heure)
 */
typedef struct {
  uint8_t hour, min, sec; // 0..23, 0..59, 0..59
  uint8_t day, month;     // 1..12
  uint16_t year;          // 20xx
} rtc_time_t;

/* STRUCTURE DE DONNÉES : bmp280_data_t
 * ------------------------------------------------------------
 * Données brutes lues depuis le capteur BMP280 :
 *  - Pression atmosphérique en Pa * 100
 *  - Température en °C * 100
 * 
 * ┌─────────────┬──────────┬──────────────┬──────────────────────────┐
 * │   Mesure    │ Unité    │   Plage      │ Exemple (valeur typique) │
 * ├─────────────┼──────────┼──────────────┼──────────────────────────┤
 * │ Pression    │ Pa *100  │ 30000–110000 │ 101325 Pa → 10132500     │
 * │ Température │ °C *100  │ -4000–8500   │ 23.56°C → 2356           │
 * └─────────────┴──────────┴──────────────┴──────────────────────────┘
 */
typedef struct {
  int32_t pressure_pa;    // Pa * 100
  int16_t temp_c_x100;    // °C * 100
} bmp280_data_t;

/* STRUCTURE DE DONNÉES : sht30_data_t
 * ------------------------------------------------------------
 * Données brutes lues depuis le capteur SHT30 :
 *  - Humidité relative en %RH * 100
 *  - Température en °C * 100
 * 
 * ┌─────────────┬──────────┬──────────────┬──────────────────────────┐
 * │   Mesure    │ Unité    │   Plage      │ Exemple (valeur typique) │
 * ├─────────────┼──────────┼──────────────┼──────────────────────────┤
 * │ Humidité    │ %RH *100 │ 0–10000      │ 45.23 %RH → 4523         │
 * │ Température │ °C *100  │ -4000–12500  │ 22.81°C → 2281           │
 * └─────────────┴──────────┴──────────────┴──────────────────────────┘
 */
typedef struct {
  uint16_t rh_x100;       // %RH * 100
  int16_t temp_c_x100;    // °C * 100
} sht30_data_t;

/* STRUCTURE DE DONNÉES : sensor_data_t
 * ------------------------------------------------------------
 * Format "full décodé" (dé-réduit) :
 *  - issu de la décompression des 3 octets enregistrés (t8, rh8, p8)
 *  - valeurs reconstruites en unités physiques classiques
 *  - précision limitée par la quantification d'encodage :
 *      Température : pas de 0.5 °C
 *      Humidité    : pas de 0.5 %
 *      Pression    : pas de 2 hPa (200 Pa)
 *
 */
typedef struct {
  int16_t  t_c_x100;   // Température en °C×100  [-4000 .. +8750]
  uint16_t rh_x100;    // Humidité en %×100      [0 .. 10000]
  int32_t  p_pa;       // Pression en Pascal     [30000 .. 110000]
} sensor_data_t;

/* CODE D'ERREUR GÉNÉRAL APPLICATIF
 *
 */
typedef enum {
  APP_OK = 0,            // success
  APP_EBUS,              // I2C bus error
  APP_EDEV,              // device not found
  APP_EPARAM,            // invalid parameter
  APP_ENCONF,            // empty configuration
  APP_EFULL              // log storage full
} app_err_t;

#endif // TYPES_H
