// ===============================================
// File:   board.c
// Desc:   Configuration matérielle du PIC18F26K83
//         - Oscillateur interne/externe
//         - Configuration des broches (TRIS)
//         - PPS (Peripheral Pin Select)
//         - Définition de _XTAL_FREQ
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "board.h"
#include <xc.h>

// ===============================================
// FONCTION : board_configure_pins
// ===============================================
// Configure les directions des broches
//
// MAPPAGE DES BROCHES (PIC18F26K83) :
//   I2C2 (Capteurs SHT30, BMP280, RTC DS1307)
//     - RB1 : SCL2 (I2C Clock)
//     - RB2 : SDA2 (I2C Data)
//
//   SPI1 (EEPROM M93C66)
//     - RC3 : SCK  (SPI Clock)
//     - RC4 : SDI  (SPI Data In - MISO)
//     - RC5 : SDO  (SPI Data Out - MOSI)
//     - RB3 : CS   (Chip Select, actif bas)
//
//   UART1 (Communication PC via USB-UART)
//     - RC6 : TX1 (Transmit)
//     - RC7 : RX1 (Receive)
//
//   UART2 (Communication Bluetooth HC-05)
//     - RB6 : TX2 (Transmit)
//     - RB7 : RX2 (Receive)
//
//   LCD (Affichage 2x16)
//     - RA0 : RS  (Register Select)
//     - RA1 : EN  (Enable)
//     - RA2 : D4  (Data bit 4)
//     - RA3 : D5  (Data bit 5)
//     - RA4 : D6  (Data bit 6)
//     - RA5 : D7  (Data bit 7)
//
//   Boutons (Menu navigation)
//     - RA6 : BTN_UP
//     - RA7 : BTN_DOWN
//     - RC0 : BTN_SELECT
//     - RC1 : BTN_BACK
//
//   LEDs (Debug)
//     - RB4 : LED1
//     - RB5 : LED2
//     - RC2 : LED3
//
// ===============================================
void board_configure_pins(void) {
    // Désactiver les entrées analogiques (tout en digital)
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    // Configuration PORTA (LCD + Boutons)
    TRISA = 0b11000000;  // RA7, RA6 en entrée (boutons), reste en sortie (LCD)
    
    // Configuration PORTB
    TRISB = 0b10000000;  // RB7 en entrée (RX2), reste en sortie
    // RB1, RB2 seront gérés par I2C (open-drain)
    // RB3 = CS (sortie)
    // RB4, RB5 = LEDs (sortie)
    // RB6 = TX2 (sortie)
    
    // Configuration open-drain pour I2C
    ODCONB = 0b00000110;  // RB1 et RB2 en drain ouvert pour I2C
    
    // Configuration PORTC
    TRISC = 0b10010011;  // RC7 (RX1), RC4 (SDI), RC0, RC1 en entrée
                         // RC6 (TX1), RC5 (SDO), RC3 (SCK), RC2 (LED) en sortie
    
    // Configuration des pull-ups pour les boutons
    WPUA = 0b11000000;  // Pull-up sur RA7 (BTN_DOWN) et RA6 (BTN_UP)
    WPUC = 0b00000011;  // Pull-up sur RC1 (BTN_BACK) et RC0 (BTN_ENTER)
    
    // Activer les pull-ups faibles
    WPUE = 0x00;        // Pas de weak pull-up enable global nécessaire pour PORTA/C
    
    // États initiaux
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    
    LATBbits.LATB3 = 1;  // CS inactif (HIGH)
}

// ===============================================
// FONCTION : board_configure_pps
// ===============================================
// Configure le Peripheral Pin Select (PPS)
//
// MAPPAGE :
//   UART1 : RC6 (TX1), RC7 (RX1)
//   UART2 : RB6 (TX2), RB7 (RX2)
//   SPI1  : RC3 (SCK), RC4 (SDI), RC5 (SDO)
//   I2C2  : RB1 (SCL2), RB2 (SDA2)
//
// ===============================================
void board_configure_pps(void) {
    // Configuration UART1 (PC <-> PIC via USB-UART)
    U1RXPPS = 0b00010111;  // RC7 = RX1 (PORTC pin 7)
    RC6PPS  = 0b00010011;  // RC6 = TX1
    
    // Configuration UART2 (Bluetooth HC-05)
    U2RXPPS = 0b00001111;  // RB7 = RX2 (PORTB pin 7)
    RB6PPS  = 0b00010110;  // RB6 = TX2
    
    // Configuration SPI1 (EEPROM M93C66)
    SPI1SDIPPS = 0b00010100;  // RC4 = SDI (PORTC pin 4)
    RC5PPS     = 0b00011111;  // RC5 = SDO
    RC3PPS     = 0b00011110;  // RC3 = SCK
    
    // Configuration I2C2 (Capteurs + RTC)
    I2C2SCLPPS = 0b00001001;  // RB1 = SCL2 (PORTB pin 1)
    I2C2SDAPPS = 0b00001010;  // RB2 = SDA2 (PORTB pin 2)
    RB1PPS     = 0b00100011;  // RB1 = SCL2 output
    RB2PPS     = 0b00100100;  // RB2 = SDA2 output
}

// ===============================================
// FONCTION : board_init
// ===============================================
// Initialisation complète du matériel
// ===============================================
void board_init(void) {
    board_configure_pins();
    board_configure_pps();
}