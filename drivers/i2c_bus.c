// ===============================================
// File:   i2c_bus.c
// Desc:   Driver I2C pour PIC18F26K83
//         Communication avec RTC DS1307, SHT30, BMP280
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "i2c_bus.h"
#include <xc.h>
#include <stdint.h>

// ===============================================
// CONSTANTES PRIVÉES
// ===============================================
#define I2C_TIMEOUT 10000u // Timeout pour les opérations I2C (boucles d’attente)

// ===============================================
// FONCTION : i2c_bus_init
// ===============================================
// Initialise le module I2C2 en mode Master
// (config minimale – le reste est fait ailleurs : TRIS/ANSEL/ODCON/PPS)
// ===============================================
app_err_t i2c_bus_init(void) {
    // Même config que dans l’exemple du prof
    I2C2CON0 = 0b10000100;  // I2C ON ; Master mode, 7-bit address
    I2C2CLK  = 0b00000011;  // CLK = MFINTOSC (500kHz)
    // Les autres registres restent à leurs valeurs par défaut
    return APP_OK;
}

// ===============================================
// FONCTION : i2c2_write
// ===============================================
// Écriture de données sur le bus I2C
//
//   addr8w : Adresse I2C 8 bits avec bit W=0 (écriture)
//   buf    : Buffer de données à écrire
//   n      : Nombre d'octets à écrire
//
// SÉQUENCE I2C :
//   [START] → [ADDR+W] → [ACK] → [DATA0] → [ACK] → ... → [STOP]
// ===============================================
app_err_t i2c2_write(uint8_t addr8w, const uint8_t *buf, uint8_t n) {
    uint8_t i;
    uint16_t timeout;

    if (n == 0) {
        return APP_OK;
    }

    // Pas de repeated start
    I2C2CON0bits.RSEN = 0;

    // Nombre d’octets à transmettre (adresse registre + data)
    I2C2CNT = n;

    // Adresse du composant en écriture
    I2C2ADB1 = addr8w;

    // Premier octet à transmettre (souvent adresse de registre)
    I2C2TXB = buf[0];

    // START
    I2C2CON0bits.S = 1;

    // Attendre l’envoi du START
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.SCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.SCIF = 0;   // Clear flag START

    // Attendre que l’adresse + premier octet soient partis
    timeout = I2C_TIMEOUT;
    while (I2C2STAT1bits.TXBE == 0) {
        if (--timeout == 0) return APP_EBUS;
    }

    // Vérifier si le slave a NACKé l’adresse
    if (I2C2CON1bits.ACKSTAT) {
        // ACKSTAT = 1 => NACK
        return APP_EDEV;
    }

    // Envoyer les octets suivants
    for (i = 1; i < n; i++) {
        I2C2TXB = buf[i];

        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.TXBE == 0) {
            if (--timeout == 0) return APP_EBUS;
        }

        // Si le slave NACK un data, on considère erreur bus
        if (I2C2CON1bits.ACKSTAT) {
            return APP_EBUS;
        }
    }

    // Attendre le STOP (envoyé automatiquement quand le compteur I2C2CNT arrive à 0)
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;   // Clear flag STOP

    return APP_OK;
}

// ===============================================
// FONCTION : i2c2_write_read
// ===============================================
// Écriture puis lecture sur le bus I2C
//
//   addr8w : Adresse I2C 8 bits en écriture (bit 0 = 0)
//   w      : Buffer d'écriture (ex: [reg_addr])
//   wn     : Nombre d'octets à écrire
//   addr8r : Adresse I2C 8 bits en lecture (bit 0 = 1)
//   r      : Buffer de lecture
//   rn     : Nombre d'octets à lire
//
// SÉQUENCE I2C :
//   [START] → [ADDR+W] → [ACK] → [DATA_W...] → [STOP]
//   [START] → [ADDR+R] → [ACK] → [DATA_R...] → [STOP]
// (fonctionne comme les blocs DS1307/BMP de l’exemple)
// ===============================================
app_err_t i2c2_write_read(uint8_t addr8w, const uint8_t *w, uint8_t wn,
                          uint8_t addr8r, uint8_t *r, uint8_t rn) {
    uint8_t i;
    uint16_t timeout;

    if (rn == 0) {
        // Si rien à lire, on fait juste un write classique
        return i2c2_write(addr8w, w, wn);
    }

    // ==========================
    // PHASE ÉCRITURE (adresse de registre)
    // ==========================
    I2C2CON0bits.RSEN = 0;    // Pas de repeated start géré ici
    I2C2CNT = wn;             // Nombre d’octets à envoyer
    I2C2ADB1 = addr8w;        // Adresse composant en écriture

    if (wn > 0) {
        I2C2TXB = w[0];
    }

    // START
    I2C2CON0bits.S = 1;

    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.SCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.SCIF = 0;

    // Attendre fin envoi de l’adresse
    timeout = I2C_TIMEOUT;
    while (I2C2STAT1bits.TXBE == 0) {
        if (--timeout == 0) return APP_EBUS;
    }

    // NACK sur adresse ?
    if (I2C2CON1bits.ACKSTAT) {
        return APP_EDEV;
    }

    // Envoyer éventuellement les octets d’écriture restants
    for (i = 1; i < wn; i++) {
        I2C2TXB = w[i];

        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.TXBE == 0) {
            if (--timeout == 0) return APP_EBUS;
        }

        if (I2C2CON1bits.ACKSTAT) {
            return APP_EBUS;
        }
    }

    // Attendre STOP de fin de phase écriture
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;

    // ==========================
    // PHASE LECTURE
    // ==========================
    I2C2ADB1 = addr8r;   // Adresse en lecture
    I2C2CNT = rn;        // Nombre d’octets à lire

    // START (comme dans les blocs du prof : start, pas RSEN)
    I2C2CON0bits.S = 1;

    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.SCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.SCIF = 0;

    // Si le slave NACKe l’adresse de lecture
    if (I2C2CON1bits.ACKSTAT) {
        return APP_EDEV;
    }

    // Comme dans l’exemple : préparation de l’ACK
    I2C2CON1bits.ACKCNT = 1;

    // Lecture des octets
    for (i = 0; i < rn; i++) {
        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.RXBF == 0) {
            if (--timeout == 0) return APP_EBUS;
        }
        r[i] = I2C2RXB;
    }

    // Le STOP est envoyé automatiquement après la réception du dernier octet
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;

    return APP_OK;
}