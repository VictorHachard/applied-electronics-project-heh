// ===============================================
// File:   lcd.c
// Desc:   Driver pour afficheur LCD 16x2 en mode 4 bits
// Groupe: G0 (Coordinateurs)
// ===============================================

#include "lcd.h"

// -----------------------------------------------------------------------------
// Envoi d'un nibble (4 bits) sur D4..D7
// data : bits 0..3 utilisés
// -----------------------------------------------------------------------------
void Lcd_Port(char data)
{
    if (data & 0x01)
        D4 = 1;
    else
        D4 = 0;

    if (data & 0x02)
        D5 = 1;
    else
        D5 = 0;

    if (data & 0x04)
        D6 = 1;
    else
        D6 = 0;

    if (data & 0x08)
        D7 = 1;
    else
        D7 = 0;
}

// -----------------------------------------------------------------------------
// Envoi d'une commande 8 bits (mode 4 bits)
// -----------------------------------------------------------------------------
void Lcd_Cmd(char cmd)
{
    // RS = 0 pour une commande
    RS = 0;

    // High nibble
    Lcd_Port(cmd >> 4);
    EN1 = 1;
    __delay_ms(2);
    EN1 = 0;

    // Low nibble
    Lcd_Port(cmd & 0x0F);
    EN1 = 1;
    __delay_ms(2);
    EN1 = 0;
}

// -----------------------------------------------------------------------------
// Effacement complet de l'afficheur
// -----------------------------------------------------------------------------
void Lcd_Clear(void)
{
    Lcd_Cmd(0x01);      // Clear display
    __delay_ms(2);
}

// -----------------------------------------------------------------------------
// Position du curseur
// row = 0 ou 1 (1ère ou 2ème ligne)
// column = 0..15
// -----------------------------------------------------------------------------
void Lcd_Set_Cursor(char row, char column)
{
    char address;

    if (row == 0)
        address = 0x80 + column;   // Première ligne
    else
        address = 0xC0 + column;   // Deuxième ligne

    Lcd_Cmd(address);
}

// -----------------------------------------------------------------------------
// Initialisation de l'afficheur LCD en 4 bits, 2 lignes, 5x8
// -----------------------------------------------------------------------------
void Lcd_Init(void)
{
    __delay_ms(20);     // Temps de démarrage LCD

    // Séquence d'initialisation standard en 4 bits
    Lcd_Cmd(0x02);      // Retour à l'accueil 4 bits
    Lcd_Cmd(0x28);      // 4 bits, 2 lignes, 5x8
    Lcd_Cmd(0x0C);      // Display ON, cursor OFF, blink OFF
    Lcd_Cmd(0x06);      // Entry mode : incrément, pas de shift
    Lcd_Clear();
}

// -----------------------------------------------------------------------------
// Envoi d'un caractère
// -----------------------------------------------------------------------------
void Lcd_Write_Char(char data)
{
    RS = 1; // Donnée

    // High nibble
    Lcd_Port(data >> 4);
    EN1 = 1;
    __delay_us(40);
    EN1 = 0;

    // Low nibble
    Lcd_Port(data & 0x0F);
    EN1 = 1;
    __delay_us(40);
    EN1 = 0;
}

// -----------------------------------------------------------------------------
// Envoi d'une chaîne terminée par '\0'
// -----------------------------------------------------------------------------
void Lcd_Write_String(const char *str)
{
    while (*str != '\0')
    {
        Lcd_Write_Char(*str);
        str++;
    }
}

// -----------------------------------------------------------------------------
// Décalage de l'affichage vers la droite
// -----------------------------------------------------------------------------
void Lcd_Shift_Right(void)
{
    Lcd_Cmd(0x1C);   // Shift display right
}

// -----------------------------------------------------------------------------
// Décalage de l'affichage vers la gauche
// -----------------------------------------------------------------------------
void Lcd_Shift_Left(void)
{
    Lcd_Cmd(0x18);   // Shift display left
}
