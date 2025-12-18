// ===============================================
// File:   bmp280.c
// Desc:   Source file for bmp280.c
// ===============================================

#include "bmp280.h"
#include "../drivers/i2c_bus.h"
#include "../core/board.h"
#include <xc.h>
#include <stdio.h>

// Adresse I2C du BMP280 : 0x76 << 1 = 0xEC (write) / 0xED (read)
// Si SDO = VDDIO alors adresse = 0x77 (0xEE/0xEF)
#define BMP280_ADDR_WRITE   0xEC
#define BMP280_ADDR_READ    0xED

// Registres importants
#define BMP280_REG_ID         0xD0
#define BMP280_REG_STATUS     0xF3
#define BMP280_REG_CTRL_MEAS  0xF4
#define BMP280_REG_CONFIG     0xF5
#define BMP280_REG_PRESS_MSB  0xF7

// timeout en ms pour la conversion (sécurité)
#define BMP280_MEAS_TIMEOUT_MS  50

#define BMP280_REG_DIG_T1   0x88   // start of block
#define BMP280_CALIB_LENGTH 24     // 12 coefficients * 2 bytes

typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} bmp280_calib_t;

app_err_t bmp280_init(void)
{
    app_err_t err;
    uint8_t buf[2];

    uint8_t chip_id;

    // Vérifier la présence du BMP280 en lisant le registre ID
    uint8_t reg_id = BMP280_REG_ID;
    err = i2c2_write_read(BMP280_ADDR_WRITE, &reg_id, 1, BMP280_ADDR_READ, &chip_id, 1);
    if (err != APP_OK) {
        // Module non présent ou erreur I2C
        return APP_EDEV;
    }

    // Vérifier que l'ID est correct (0x58 pour BMP280)
    if (chip_id != 0x58) {
        // Mauvais chip ID, ce n'est pas un BMP280
        return APP_EDEV;
    }

    // ------------------------------
    // REGISTRE 0xF4 : ctrl_meas
    // ------------------------------
    // osrs_t = 001 (Ã1)
    // osrs_p = 001 (Ã1)
    // mode   = 01  (forced)
    //
    // b7..5  b4..2  b1..0
    //  001    001     01     = 0b00100101 = 0x25
    buf[0] = BMP280_REG_CTRL_MEAS;
    buf[1] = 0x25;

    err = i2c2_write(BMP280_ADDR_WRITE, buf, 2);
    if (err != APP_OK) return err;

    // ------------------------------
    // REGISTRE 0xF5 : config
    // ------------------------------
    // filter = 000 (off)
    // tstandby = 000 (0.5ms, ignorÃ© en forced mode)
    // resulting byte = 0x00
    buf[0] = BMP280_REG_CONFIG;
    buf[1] = 0x00;

    err = i2c2_write(BMP280_ADDR_WRITE, buf, 2);
    if (err != APP_OK) return err;
    
    return APP_OK;
}

app_err_t bmp280_read_raw(int32_t *rawTemp, int32_t *rawPress)
{
    app_err_t err;
    uint8_t wbuf[2];
    uint8_t reg;
    uint8_t rbuf[6];

    // 1) retrigger forced measurement :
    //    osrs_t = 1, osrs_p = 1, mode = forced -> 0x25
    bmp280_init();

    // 2) attendre la fin de conversion en lisant le registre STATUS (0xF3 bit 3 = measuring)
    reg = BMP280_REG_STATUS;
    uint16_t to = BMP280_MEAS_TIMEOUT_MS;
    uint8_t status = 0xFF;

    while (to--)
    {
        // write reg, then read 1 byte (addr8w, w, wn=1, addr8r, r, rn=1)
        err = i2c2_write_read(BMP280_ADDR_WRITE, &reg, 1, BMP280_ADDR_READ, &status, 1);
        if (err != APP_OK) return err;

        // bit 3 (0x08) -> measuring flag; 0 => conversion finie
        if ((status & 0x08) == 0) break;

        __delay_ms(1);
    }

    if ((status & 0x08) != 0)
    {
        // timeout expired, device still measuring
        return APP_EBUS; // ou APP_EDEV selon ta convention ; j'utilise APP_EBUS pour timeout
    }

    // 3) Burst read des 6 octets (0xF7 .. 0xFC)
    reg = BMP280_REG_PRESS_MSB;
    err = i2c2_write_read(BMP280_ADDR_WRITE, &reg, 1, BMP280_ADDR_READ, rbuf, 6);
    if (err != APP_OK) return err;

    // 4) Reconstruire valeurs brutes 20 bits (unsigned)
    // pression : rbuf[0] = press_msb, rbuf[1] = press_lsb, rbuf[2] = press_xlsb
    *rawPress = ((int32_t)rbuf[0] << 12) |
                ((int32_t)rbuf[1] << 4)  |
                ((int32_t)(rbuf[2] >> 4) & 0x0F);

    // température : rbuf[3]..rbuf[5]
    *rawTemp  = ((int32_t)rbuf[3] << 12) |
                ((int32_t)rbuf[4] << 4)  |
                ((int32_t)(rbuf[5] >> 4) & 0x0F);

    return APP_OK;
}


int32_t bmp280_compensate_T_int32(int32_t adc_T, bmp280_calib_t *calib, int32_t *t_fine)
{
    int32_t var1, var2, T;

    var1 = ((((adc_T >> 3) - ((int32_t)calib->dig_T1 << 1))) * ((int32_t)calib->dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)calib->dig_T1)) * ((adc_T >> 4) - ((int32_t)calib->dig_T1))) >> 12) * ((int32_t)calib->dig_T3)) >> 14;

    *t_fine = var1 + var2;
    T = (*t_fine * 5 + 128) >> 8;

    return T; // Température en °C ×100 (ex : 5123 = 51.23°C)
}


// Returns pressure in Pa as unsigned 32 bit integer. Output value of "96386" equals 96386 Pa = 963.86 hPa
uint32_t bmp280_compensate_P_int32(int32_t adc_P, bmp280_calib_t *calib, int32_t *t_fine)
{
    int32_t var1, var2;
    uint32_t p;

    var1 = (*t_fine >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)calib->dig_P6;
    var2 = var2 + ((var1 * (int32_t)calib->dig_P5) << 1);
    var2 = (var2 >> 2) + ((int32_t)calib->dig_P4 << 16);
    var1 = (((calib->dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3)
         + ((((int32_t)calib->dig_P2) * var1) >> 1)) >> 18;
    var1 = ((32768 + var1) * (int32_t)calib->dig_P1) >> 15;

    if (var1 == 0) {
        return 0; // avoid division by zero
    }

    p = ((uint32_t)((int32_t)1048576 - adc_P) - (var2 >> 12)) * 3125;

    if (p < 0x80000000) {
        p = (p << 1) / (uint32_t)var1;
    } else {
        p = (p / (uint32_t)var1) * 2;
    }

    var1 = ((int32_t)calib->dig_P9 * (int32_t)(((p >> 3) * (p >> 3)) >> 13)) >> 12;
    var2 = (((int32_t)(p >> 2)) * (int32_t)calib->dig_P8) >> 13;

    p = (uint32_t)((int32_t)p + ((var1 + var2 + calib->dig_P7) >> 4));

    return p;
}


app_err_t bmp280_read_calibration(bmp280_calib_t *cal)
{
    app_err_t err;
    uint8_t reg = BMP280_REG_DIG_T1;
    uint8_t buf[BMP280_CALIB_LENGTH];

    // Lecture burst de 24 octets
    err = i2c2_write_read(BMP280_ADDR_WRITE, &reg, 1, BMP280_ADDR_READ, buf, BMP280_CALIB_LENGTH);
    if (err != APP_OK)
        return err;

    // Reconstruction selon datasheet
    cal->dig_T1 = (uint16_t)(buf[1] << 8 | buf[0]);
    cal->dig_T2 = (int16_t)(buf[3] << 8 | buf[2]);
    cal->dig_T3 = (int16_t)(buf[5] << 8 | buf[4]);

    cal->dig_P1 = (uint16_t)(buf[7]  << 8 | buf[6]);
    cal->dig_P2 = (int16_t)(buf[9]   << 8 | buf[8]);
    cal->dig_P3 = (int16_t)(buf[11]  << 8 | buf[10]);
    cal->dig_P4 = (int16_t)(buf[13] << 8 | buf[12]);
    cal->dig_P5 = (int16_t)(buf[15] << 8 | buf[14]);
    cal->dig_P6 = (int16_t)(buf[17] << 8 | buf[16]);
    cal->dig_P7 = (int16_t)(buf[19] << 8 | buf[18]);
    cal->dig_P8 = (int16_t)(buf[21] << 8 | buf[20]);
    cal->dig_P9 = (int16_t)(buf[23] << 8 | buf[22]);

    return APP_OK;
}


app_err_t bmp280_read(bmp280_data_t* data)
{
    app_err_t err;

    err = bmp280_init();
    if (err != APP_OK)
        return err;
    
    int32_t rawT, rawP;
    int32_t t_fine;
    bmp280_calib_t cal;

    // 1) Lire RAW
    err = bmp280_read_raw(&rawT, &rawP);
    if (err != APP_OK)
        return err;

    // 2) Lire coefficients
    err = bmp280_read_calibration(&cal);
    if (err != APP_OK)
        return err;

    // 3) Compensation température
    int32_t T = bmp280_compensate_T_int32(rawT, &cal, &t_fine);
    data->temp_c_x100 = T;
    
    // 4) Compensation pression (INT64 VERSION)
    int32_t P = bmp280_compensate_P_int32(rawP, &cal, &t_fine);
    data->pressure_pa = (int32_t)P;

    return APP_OK;
}