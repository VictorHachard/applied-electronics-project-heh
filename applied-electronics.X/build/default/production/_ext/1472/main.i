# 1 "../main.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "../main.c" 2
# 12 "../main.c"
#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINTOSC_64MHZ


#pragma config CLKOUTEN = OFF
#pragma config PR1WAY = OFF
#pragma config CSWEN = ON
#pragma config FCMEN = ON


#pragma config MCLRE = EXTMCLR
#pragma config PWRTS = PWRT_OFF
#pragma config MVECEN = OFF
#pragma config IVT1WAY = OFF
#pragma config LPBOREN = OFF
#pragma config BOREN = SBORDIS


#pragma config BORV = VBOR_2P45
#pragma config ZCD = OFF
#pragma config PPS1WAY = OFF
#pragma config STVREN = ON
#pragma config DEBUG = OFF
#pragma config XINST = OFF


#pragma config WDTCPS = WDTCPS_31
#pragma config WDTE = OFF


#pragma config WDTCWS = WDTCWS_7
#pragma config WDTCCS = SC


#pragma config BBSIZE = BBSIZE_512
#pragma config BBEN = OFF
#pragma config SAFEN = OFF
#pragma config WRTAPP = OFF


#pragma config WRTB = OFF
#pragma config WRTC = OFF
#pragma config WRTD = OFF
#pragma config WRTSAF = OFF
#pragma config LVP = OFF


#pragma config CP = OFF



# 1 "../app/app_main.h" 1
# 13 "../app/app_main.h"
void app_main_init(void);



void app_main_loop(void);
# 64 "../main.c" 2
# 75 "../main.c"
void main(void) {
    app_main_init();
    app_main_loop();
}
