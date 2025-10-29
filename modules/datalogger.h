// ===============================================
// File:   datalogger.h
// Desc:   Header file for datalogger.h
// ===============================================

#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <stdint.h>
#include <stdbool.h>
#include "core/types.h"

typedef struct {
  uint16_t sample_period_s; // Δt
  uint16_t max_records;     // borne anti-débordement
  bool     running;
} dl_cfg_t;

void dl_init(const dl_cfg_t *cfg);

void dl_start(void);

void dl_stop(void);

void dl_clear(void); // clear EEPROM

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

/* STRUCTURE DE DONNÉES : dl_header_t
 * ------------------------------------------------------------
 * L’en-tête (header) du datalogger est stocké au début de l’EEPROM
 * et décrit les paramètres de la session d’enregistrement.
 * Il tient sur 6 octets, tous codés en 8 bits.
 *
 * ┌──────────────┬────────┬──────────────────────────────┬─────────────────────────────┬───────────────┐
 * │   Champ      │ Octets │ Encodage / Format            │ Signification               │ Plage / Unité │
 * ├──────────────┼────────┼──────────────────────────────┼─────────────────────────────┼───────────────┤
 * │ t0_min_le[3] │   3    │ t0_min = minute de départ    │ Horodatage de départ        │ 0 .. 16 777 215 min |
 * │ dt_s         │   1    │ Période d’échantillonnage    │ Intervalle entre mesures    │ 1 .. 255 s     |
 * │ flags        │   1    │ Bits d’options (voir ci-dessous) │ Activation pression, unités… │ 0x00 .. 0xFF   |
 * │ count        │   1    │ Nombre de mesures valides     │ Nb d’échantillons écrits    │ 0 .. 255       |
 * └──────────────┴────────┴──────────────────────────────┴─────────────────────────────┴───────────────┘
 *
 * Détails :
 * - t0_min_le[3] est un entier 24 bits little endian :
 *      t0_min = t0_min_le[0] | (t0_min_le[1] << 8) | (t0_min_le[2] << 16)
 *   → représente le nombre total de minutes écoulées depuis une référence
 *     (par exemple 0 = 00:00 au jour du démarrage).
 *
 * - dt_s : définit la période d’échantillonnage en secondes.
 *     Exemple : dt_s = 10 → un log toutes les 10 secondes.
 *
 * - flags : permet d’activer des options de session.
 *     Bit 0 : HAS_PRESSURE → 1 si la pression est enregistrée
 *     Bit 1 : UNUSED (réservé)
 *     Bit 2–7 : futurs usages (CRC, format, unités, etc.)
 *
 * - count : indique combien d’échantillons ont été écrits
 *     (sert à calculer la prochaine adresse libre en EEPROM).
 *
 * Exemple :
 *   t0_min_le = {0x34, 0x12, 0x00}   → t0_min = 0x001234 = 4660 min
 *   dt_s = 10                        → intervalle de 10 s
 *   flags = 0x01                     → pression activée
 *   count = 15                       → 15 mesures loguées
 *
 * Adresse EEPROM :
 *   Base header  : 0x000
 *   Base records : 0x006 (après les 6 octets du header)
 *
 */
typedef struct __attribute__((packed)) {
    uint8_t t0_min_le[3];  // minute de départ (24-bit LE)
    uint8_t dt_s;          // période d’échantillonnage en secondes (1–255)
    uint8_t flags;         // options : bit0 = HAS_PRESSURE, autres réservés
    uint8_t count;         // nombre de mesures enregistrées (0–255)
} dl_header_t;

app_err_t dl_push_record(const sensor_data_t *rec);

uint16_t dl_count(void);

app_err_t dl_read(uint16_t index, sensor_data_t *rec);

#endif // DATALOGGER_H
