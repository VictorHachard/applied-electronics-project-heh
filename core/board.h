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

// Initialisation complète du matériel
void board_init(void);

// Configuration des broches (TRIS, ANSEL)
void board_configure_pins(void);

// Configuration du Peripheral Pin Select (PPS)
void board_configure_pps(void);

#endif // BOARD_H
