// ===============================================
// File:   main.c
// Desc:   Point d'entrée principal du programme
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "app/app_main.h"
#include <xc.h>

// ===============================================
// BITS DE CONFIGURATION DU PIC18F47Q10
// ===============================================

// CONFIG1L
#pragma config FEXTOSC = OFF        // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ  // Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

// CONFIG1H
#pragma config CLKOUTEN = OFF       // Clock out Enable (CLKOUT function is disabled)
#pragma config PR1WAY = ON          // PRLOCKED One-Way Set Enable (PRLOCKED bit can be cleared and set only once)
#pragma config CSWEN = ON           // Clock Switch Enable (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON           // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR      // MCLR Enable (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR)
#pragma config PWRTS = PWRT_OFF     // Power-up timer selection (PWRT is disabled)
#pragma config MVECEN = ON          // Multi-vector enable (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON         // IVTLOCK bit One-way set enable (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF        // Low Power BOR Enable (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS      // Brown-out Reset Enable (Brown-out Reset enabled, SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_1P9      // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF            // ZCD Disable (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON         // PPSLOCK bit One-Way Set Enable (PPSLOCKED bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON          // Stack Full/Underflow Reset Enable (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF          // Debugger Enable (Background debugger disabled)
#pragma config XINST = OFF          // Extended Instruction Set Enable (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31   // WDT Period selection (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF           // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC          // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512  // Boot Block Size selection (Boot Block size is 512 words)
#pragma config BBEN = OFF           // Boot Block enable (Boot block disabled)
#pragma config SAFEN = OFF          // Storage Area Flash enable (SAF disabled)
#pragma config WRTAPP = OFF         // Application Block write protection (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF           // Boot Block Write Protection (Boot Block not write-protected)
#pragma config WRTC = OFF           // Configuration Register Write Protection (Configuration registers not write-protected)
#pragma config WRTD = OFF           // Data EEPROM Write Protection (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF         // SAF Write protection (SAF not Write Protected)
#pragma config LVP = ON             // Low Voltage Programming Enable (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF             // PFM and Data EEPROM Code Protection (PFM and Data EEPROM code protection disabled)

// ===============================================
// FONCTION : main
// ===============================================
// Point d'entrée principal du programme
// 
// ARCHITECTURE :
//   1. Initialisation complète (app_main_init)
//   2. Boucle infinie (app_main_loop)
//
// ===============================================
void main(void) {
    app_main_init();
    app_main_loop();
}
