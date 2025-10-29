# applied-electronics-project-heh

## Arborescence proposée

```c
/firmware
  /app
    app_main.h
    app_main.c            // (G0) boucle, scheduler, routeur d’évènements
    app_menu.h            // (G1) signatures écran/menu
  /drivers
    i2c_bus.h             // (G0) thin wrapper I2C commun
    spi_bus.h             // (G0) thin wrapper SPI commun
    uart_pc.h             // 
    uart_bt.h             // (G5) UART2 (HC-05)
    lcd.h                 // (fourni)
  /modules
    rtc_ds1307.h          // (G1)
    bmp280.h              // (G2)
    sht30.h               // (G3)
    eeprom_m93c66.h       // (G4)
    datalogger.h          // (G4)
    bluetooth_proto.h     // (G5)
  /core
    board.h               // (G0) mapping broches, PPS, clocks, _XTAL_FREQ
    isr.h                 // (G0) point d’entrée ISR unique
    types.h               // (G0) types partagés (timestamp, mesures, erreurs)
```

## Types partagés

Dans le fichier `core/types.h` il y a des définitions de types partagés entre plusieurs modules.

## Répartition par groupe

### Groupe 0 – Coordinateurs

* `board.c/h` → configuration du PIC (oscillateur, TRIS, PPS, interruptions)
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
