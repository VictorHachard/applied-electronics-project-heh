// ===============================================
// File:   sht30.c
// Desc:   Source file for SHT30 (PIC18F26K83 / I2C2 registers)
// ===============================================

#include "sht30.h"
#include "../drivers/i2c_bus.h"  // Utiliser le module I2C centralisé
#include "../core/board.h"
#include <xc.h>
#include <stdint.h>

#define SHT30_ADDR_WRITE  0x88
#define SHT30_ADDR_READ   0x89

// Single shot measurement, High repeatability
#define SHT30_CMD_MSB     0x24
#define SHT30_CMD_LSB     0x00

// Read status register command
#define SHT30_STATUS_MSB  0xF3
#define SHT30_STATUS_LSB  0x2D

#define SHT30_MEAS_DELAY_MS  15

// CRC-8 (poly 0x31, init 0xFF)
static uint8_t sht30_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

app_err_t sht30_init(void)
{
    app_err_t err;
    uint8_t status_cmd[2] = {SHT30_STATUS_MSB, SHT30_STATUS_LSB};
    uint8_t status_buf[3];
    
    // Vérifier la présence du SHT30 en lisant le registre de status
    // Utiliser write_read pour envoyer la commande et lire la réponse
    err = i2c2_write_read(SHT30_ADDR_WRITE, status_cmd, 2, 
                          SHT30_ADDR_READ, status_buf, 3);
    if (err != APP_OK) {
        // Module non présent ou erreur I2C
        return APP_EDEV;
    }
    
    // Si on arrive ici, le module a répondu correctement
    return APP_OK;
}

app_err_t sht30_read(sht30_data_t *data)
{
    if (data == 0) {
        return APP_EPARAM;
    }

    app_err_t err;
    uint8_t cmd[2] = {SHT30_CMD_MSB, SHT30_CMD_LSB};
    uint8_t rbuf[6];  // T_MSB, T_LSB, T_CRC, RH_MSB, RH_LSB, RH_CRC

    // 1) Envoyer la commande de mesure
    err = i2c2_write(SHT30_ADDR_WRITE, cmd, 2);
    if (err != APP_OK) {
        return err;
    }

    // 2) Attendre le temps de mesure
    __delay_ms(SHT30_MEAS_DELAY_MS);

    // 3) Lire les 6 bytes de résultat
    // Utiliser write_read avec wn=0 pour faire une lecture pure
    err = i2c2_write_read(SHT30_ADDR_WRITE, NULL, 0,
                          SHT30_ADDR_READ, rbuf, 6);
    if (err != APP_OK) {
        return err;
    }

    // 4) Vérifier les CRC
    uint8_t t_data[2]  = {rbuf[0], rbuf[1]};
    uint8_t rh_data[2] = {rbuf[3], rbuf[4]};

    if (sht30_crc8(t_data, 2) != rbuf[2]) {
        return APP_EIO;  // Erreur CRC température
    }
    if (sht30_crc8(rh_data, 2) != rbuf[5]) {
        return APP_EIO;  // Erreur CRC humidité
    }

    // 5) Convertir les valeurs brutes
    uint16_t raw_temperature = ((uint16_t)rbuf[0] << 8) | rbuf[1];
    uint16_t raw_humidity    = ((uint16_t)rbuf[3] << 8) | rbuf[4];

    // Température (°C): -45 + 175 * raw / 65535
    // Stockée en °C * 100
    int32_t temp_c_x100 = -4500L + ((17500L * (int32_t)raw_temperature) / 65535L);
    data->temp_c_x100 = (int16_t)temp_c_x100;

    // Humidité (%RH): 100 * raw / 65535
    // Stockée en % * 100
    data->rh_x100 = (uint16_t)((10000UL * (uint32_t)raw_humidity) / 65535UL);

    return APP_OK;
}