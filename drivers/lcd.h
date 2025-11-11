#ifndef LCD_H
#define LCD_H

#include "../core/board.h"   // Donne RS, EN1, D4..D7 et _XTAL_FREQ

// Fonctions bas niveau
void Lcd_Port(char data);
void Lcd_Cmd(char cmd);

// Fonctions de haut niveau
void Lcd_Clear(void);
void Lcd_Set_Cursor(char row, char column);
void Lcd_Init(void);
void Lcd_Write_Char(char data);
void Lcd_Write_String(const char *str);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);

#endif // LCD_H
