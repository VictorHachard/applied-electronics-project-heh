// ===============================================
// File:   board.h
// Desc:   Configuration matérielle du PIC18F26K83
//         - Oscillateur interne/externe
//         - Configuration des broches (TRIS)
//         - PPS (Peripheral Pin Select)
//         - Définition de _XTAL_FREQ
// Groupe: G0 (Coordinateurs)
// ===============================================

#ifndef BOARD_H
#define BOARD_H

#include <xc.h>

// Fréquence d'oscillateur pour les __delay_ms/us
#define _XTAL_FREQ 64000000UL

// Mapping des broches du LCD
#define RS  PORTAbits.RA0
#define EN1 PORTAbits.RA1
#define D4  PORTAbits.RA2
#define D5  PORTAbits.RA3
#define D6  PORTAbits.RA4
#define D7  PORTAbits.RA5

// Initialisation complète du matériel
void board_init(void);

// Configuration des broches (TRIS, ANSEL)
void board_configure_pins(void);

// Configuration du Peripheral Pin Select (PPS)
void board_configure_pps(void);

#endif // BOARD_H
