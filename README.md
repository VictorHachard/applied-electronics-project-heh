# Weather Station - PIC18F26K83

## Mappage des pattes - PIC18F26K83

```
                      PIC18F26K83
                   ┌────────────────┐
    MCLR/VPP   ────┤1  RE3    RB7 28├──── RX2 (Bluetooth HC-05)
    RA0 (LCD_RS)───┤2  RA0    RB6 27├──── TX2 (Bluetooth HC-05)
    RA1 (LCD_EN)───┤3  RA1    RB5 26├──── LED2 (Debug)
    RA2 (LCD_D4)───┤4  RA2    RB4 25├──── LED1 (Debug)
    RA3 (LCD_D5)───┤5  RA3    RB3 24├──── CS (EEPROM)
    RA4 (LCD_D6)───┤6  RA4    RB2 23├──── SDA2 (I2C)
    RA5 (LCD_D7)───┤7  RA5    RB1 22├──── SCL2 (I2C)
    RA6 (BTN_UP)───┤8  RA6    RB0 21├──── (Libre)
    RA7 (BTN_DOWN)─┤9  RA7    VDD 20├──── +5V
    GND       ─────┤10 VSS    VSS 19├──── GND
    RC0 (BTN_SEL)──┤11 RC0    RC7 18├──── RX1 (USB-UART PC)
    RC1 (BTN_BACK)─┤12 RC1    RC6 17├──── TX1 (USB-UART PC)
    RC2 (LED3)─────┤13 RC2    RC5 16├──── SDO (SPI EEPROM)
    RC3 (SCK)──────┤14 RC3    RC4 15├──── SDI (SPI EEPROM)
                   └────────────────┘
```

## Arborescence du projet

```c
/firmware
  /app
    app_main.h           // (G0) Boucle principale et scheduler
    app_main.c
    app_menu.h           // (G1) Interface utilisateur et navigation
    app_menu.c
  
  /drivers
    i2c_bus.h            // (G0) Driver I2C2 pour capteurs et RTC
    i2c_bus.c
    spi_bus.h            // (G0) Driver SPI1 pour EEPROM
    spi_bus.c
    uart_pc.h            // (Bonus) Communication PC via UART1
    uart_pc.c
    uart_bt.h            // (G5) Communication Bluetooth via UART2
    uart_bt.c
    lcd.h                // (Fourni) Affichage LCD 2x16
  
  /modules
    rtc_ds1307.h         // (G1) Horloge temps réel
    rtc_ds1307.c
    bmp280.h             // (G2) Capteur pression + température
    bmp280.c
    sht30.h              // (G3) Capteur humidité + température
    sht30.c
    eeprom_m93c66.h      // (G4) Mémoire EEPROM SPI
    eeprom_m93c66.c
    datalogger.h         // (G4) Enregistrement des données
    datalogger.c
    bluetooth_proto.h    // (G5) Protocole Bluetooth
    bluetooth_proto.c
  
  /core
    board.h              // (G0) Configuration matérielle du PIC
    board.c
    isr.h                // (G0) Gestion des interruptions
    isr.c
    types.h              // (G0) Types de données partagés
  
  main.c                 // (G0) Point d'entrée du programme
```

## Types de données partagés

Dans le fichier `core/types.h` il y a des définitions de types partagés entre plusieurs modules.

## Répartition par groupe

### Groupe 0 – Coordinateurs

* `board.c/h` → configuration du PIC (TRIS, PPS, interruptions)
* `app_main.c/h` → boucle principale, scheduler, intégration des modules
* `isr.c/h` → gestion centralisée des interruptions

### Groupe 1 – Menu LCD + RTC (DS1307)

* `app_menu.c/h` → gestion des écrans, navigation, boutons
* `rtc_ds1307.c/h` → lecture/écriture du temps via I²C

### Groupe 2 – Capteur BMP280 (pression + température)

* `bmp280.c/h` → initialisation, lecture des registres, compensation

### Groupe 3 – Capteur SHT30 (humidité + température)

* `sht30.c/h` → mesure simple via I²C, conversion des valeurs

### Groupe 4 – Datalogger / EEPROM

* `eeprom_m93c66.c/h` → lecture/écriture SPI bas-niveau
* `datalogger.c/h` → gestion du header, des records, du timing, compression (reduce) et décompression (dereduce) des données

### Groupe 5 – Bluetooth (UART2 + application mobile)

* `bluetooth_proto.c/h` → réception des commandes, envoi des données (temps, mesures, logs)

### Optionnel – Liaison PC (UART1)**

* `uart_pc.c/h` → export CSV ou debug série
