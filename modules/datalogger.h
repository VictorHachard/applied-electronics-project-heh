// ===============================================
// File:   datalogger.h
// Desc:   Header file for datalogger.h
// ===============================================

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <stdint.h>
#include <stdbool.h>
#include "../core/types.h"

// Attention : la strucure de données est une proposition de G0 elle peut etre modifier car on a rechechit 10minutes

typedef struct {
  uint8_t sample_period_s;  // Δt
  uint8_t data_count;       // nombre de données enregistrées
  rtc_time_t start_time;    // heure de début
  bool running;             // état du datalogger
} dl_cfg_t;

app_err_t dl_reset_config(void);

app_err_t dl_set_sample_period_s(uint8_t period_s);

app_err_t dl_set_running(rtc_time_t *start_time);

app_err_t dl_get_config(dl_cfg_t *cfg);

/* STRUCTURE DE DONNÉES : log_rec_t
 * ------------------------------------------------------------
 * Chaque enregistrement (record) du datalogger tient sur 3 octets :
 *  - 1 octet pour la température (t8)
 *  - 1 octet pour l’humidité relative (rh8)
 *  - 1 octet pour la pression atmosphérique (p8)
 *
 * Les valeurs physiques sont compressées sur 8 bits selon le
 * schéma d’encodage suivant :
 *
 * ┌─────────────┬────────┬─────────────────────────────┬──────────────────────┬────────────┐
 * │   Mesure    │ Octets │ Encodage (→ 8 bits)         │ Plage décodée        │ Résolution │
 * ├─────────────┼────────┼─────────────────────────────┼──────────────────────┼────────────┤
 * │ Température │   1    │ t8 = (T + 40) / 0.5         │ –40.0 .. +87.5 °C    │ 0.5 °C     │
 * │ Humidité    │   1    │ rh8 = RH * 2                │ 0.0 .. 100.0 %RH     │ 0.5 %      │
 * │ Pression    │   1    │ p8 = (P_hPa – 800) / 2      │ 800 .. 1200 hPa      │ 2 hPa      │
 * └─────────────┴────────┴─────────────────────────────┴──────────────────────┴────────────┘
 *
 * Avantages :
 *  - Données ultra compactes (3 octets par mesure)
 *  - Résolutions adaptées aux capteurs réels (±0.5°C, ±0.5%, ±2hPa)
 *
 * Exemple :
 *   T = 23.5°C  → t8 = (23.5 + 40)/0.5 = 127
 *   RH = 45.0%  → rh8 = 45 * 2 = 90
 *   P = 1013hPa → p8 = (1013 - 800)/2 = 106
 *   Record stocké = [127 | 90 | 106]
 */
typedef struct {
  uint8_t t8;             // temperature in °C (encoded)
  uint8_t rh8;            // humidity in %RH (encoded)
  uint8_t p8;             // pressure in hPa (encoded)
} sensor_reduced_t;

/* Réduction (FULL → RÉDUIT, avec clamp & arrondis) */
app_err_t sensor_reduce(const sensor_data_t* in, sensor_reduced_t* out);

/* Dé-réduction (RÉDUIT → FULL APPROX)
 * Retourne un "full" quantifié :
 *   t_c_x100 : multiples de 50 (0.5 °C)
 *   rh_x100  : multiples de 50 (0.5 %)
 *   p_pa     : multiples de 200 Pa (2 hPa)
 */
void sensor_dereduce(const sensor_reduced_t* in, sensor_data_t* out);

// Écrire un enregistrement à la fin du log
// Verifie l'espace disponible avant d'écrire et verifie le flag running
app_err_t dl_push_record(const sensor_data_t *rec);

app_err_t dl_read(uint16_t index, sensor_data_t *rec);

#endif // DATALOGGER_H
