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
#define I2C_TIMEOUT 1000  // Timeout pour les opérations I2C (ms)

// ===============================================
// FONCTION : i2c_bus_init
// ===============================================
// Initialise le module I2C2 en mode Master
// ===============================================
app_err_t i2c_bus_init() {
    // Configuration I2C2
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
// PARAMÈTRES :
//   addr8w : Adresse I2C 8 bits avec bit W=0 (écriture)
//            Exemple : 0x90 pour adresse 0x48 en écriture
//   buf    : Buffer de données à écrire
//   n      : Nombre d'octets à écrire
//
// SÉQUENCE I2C :
//   [START] → [ADDR+W] → [ACK] → [DATA0] → [ACK] → ... → [STOP]
//
// RETOUR :
//   APP_OK si succès
//   APP_EBUS si erreur de bus (NACK, timeout, collision)
//
// ===============================================
app_err_t i2c2_write(uint8_t addr8w, const uint8_t *buf, uint8_t n) {
    uint8_t i;
    uint16_t timeout;
    
    // Désactiver RSEN (on ne travaille pas avec Repeated Start)
    I2C2CON0bits.RSEN = 0;
    
    // Configurer le nombre d'octets à transmettre
    I2C2CNT = n;
    
    // Charger l'adresse du périphérique
    I2C2ADB1 = addr8w;
    
    // Charger le premier octet (adresse du registre généralement)
    if (n > 0) {
        I2C2TXB = buf[0];
    }
    
    // Générer START
    I2C2CON0bits.S = 1;
    
    // Attendre l'envoi du START
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.SCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.SCIF = 0;
    
    // Attendre que le buffer soit vide (adresse envoyée)
    timeout = I2C_TIMEOUT;
    while (I2C2STAT1bits.TXBE == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    
    // Vérifier le NACK (périphérique non trouvé)
    if (I2C2CON1bits.NACKIF) {
        I2C2CON1bits.NACKIF = 0;
        return APP_EDEV;
    }
    
    // Envoyer les octets suivants
    for (i = 1; i < n; i++) {
        I2C2TXB = buf[i];
        
        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.TXBE == 0) {
            if (--timeout == 0) return APP_EBUS;
        }
        
        if (I2C2CON1bits.NACKIF) {
            I2C2CON1bits.NACKIF = 0;
            return APP_EBUS;
        }
    }
    
    // Attendre l'envoi du STOP (automatique)
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;
    
    return APP_OK;
}

// ===============================================
// FONCTION : i2c2_write_read
// ===============================================
// Écriture puis lecture sur le bus I2C
//
// PARAMÈTRES :
//   addr8w : Adresse I2C 8 bits en écriture (bit 0 = 0)
//   w      : Buffer d'écriture
//   wn     : Nombre d'octets à écrire
//   addr8r : Adresse I2C 8 bits en lecture (bit 0 = 1)
//   r      : Buffer de lecture
//   rn     : Nombre d'octets à lire
//
// SÉQUENCE I2C :
//   [START] → [ADDR+W] → [ACK] → [DATA_W] → [ACK] →
//   [RESTART] → [ADDR+R] → [ACK] → [DATA_R] → [ACK/NACK] → [STOP]
//
// RETOUR :
//   APP_OK si succès, APP_EBUS/APP_EDEV si erreur
//
// ===============================================
app_err_t i2c2_write_read(uint8_t addr8w, const uint8_t *w, uint8_t wn,
                          uint8_t addr8r, uint8_t *r, uint8_t rn) {
    uint8_t i;
    uint16_t timeout;
    
    // PHASE ÉCRITURE (envoyer l'adresse du registre)
    I2C2CON0bits.RSEN = 0;
    I2C2CNT = wn;
    I2C2ADB1 = addr8w;
    
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
    
    timeout = I2C_TIMEOUT;
    while (I2C2STAT1bits.TXBE == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    
    if (I2C2CON1bits.NACKIF) {
        I2C2CON1bits.NACKIF = 0;
        return APP_EDEV;
    }
    
    // Envoyer les octets suivants
    for (i = 1; i < wn; i++) {
        I2C2TXB = w[i];
        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.TXBE == 0) {
            if (--timeout == 0) return APP_EBUS;
        }
    }
    
    // Attendre STOP
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;
    
    // PHASE LECTURE
    I2C2ADB1 = addr8r;
    I2C2CNT = rn;
    
    // START (devient Repeated Start automatiquement)
    I2C2CON0bits.S = 1;
    
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.SCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.SCIF = 0;
    
    if (I2C2CON1bits.NACKIF) {
        I2C2CON1bits.NACKIF = 0;
        return APP_EDEV;
    }
    
    // Configurer ACK pour la réception
    I2C2CON1bits.ACKCNT = 1;
    
    // Lire les octets
    for (i = 0; i < rn; i++) {
        timeout = I2C_TIMEOUT;
        while (I2C2STAT1bits.RXBF == 0) {
            if (--timeout == 0) return APP_EBUS;
        }
        r[i] = I2C2RXB;
    }
    
    // Attendre STOP (automatique après ACK)
    timeout = I2C_TIMEOUT;
    while (I2C2PIRbits.PCIF == 0) {
        if (--timeout == 0) return APP_EBUS;
    }
    I2C2PIRbits.PCIF = 0;
    
    return APP_OK;
}