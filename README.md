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

## Organisation des fichiers

### Séparation .h / .c

Chaque module est divisé en deux fichiers :

| Fichier | Contenu | Visible par |
|---------|---------|-------------|
| `.h` (header) | Prototypes des fonctions, types, constantes | Tous les modules |
| `.c` (source) | Implémentation des fonctions | Uniquement ce module |

**Avantages :**
- Chaque groupe travaille sur son `.c` sans conflit
- Les autres groupes voient seulement les prototypes dans le `.h`
- Compilation modulaire (modification d'un `.c` ne recompile pas tout)

### Arborescence

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
    lcd.c
  
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

## Flux d'exécution

### Point d'entrée : main.c

```c
void main(void) {
    app_main_init();    // 1. Initialisation complète
    app_main_loop();    // 2. Boucle infinie (ne retourne jamais)
}
```

Le fichier `main.c` est volontairement **minimaliste**. Toute la logique est dans `app_main.c`.

### Séquence d'initialisation

```
main()
  │
  └──► app_main_init()
         │
         ├──► board_init()           // Config matérielle
         │      ├── configure_pins()    (TRIS, ANSEL)
         │      └── configure_pps()     (routage périphériques)
         │
         ├──► Lcd_Init()             // Affichage
         │
         ├──► i2c_bus_init()         // Bus I2C
         ├──► spi_bus_init()         // Bus SPI
         ├──► uart_pc_init(9600)     // UART1
         ├──► uart_bt_init(9600)     // UART2
         │
         ├──► rtc_init()             // RTC DS1307
         ├──► bmp280_init()          // Capteur pression
         ├──► sht30_init()           // Capteur humidité
         ├──► eeprom_init()          // EEPROM
         │
         ├──► dl_get_config()        // Charger config datalogger
         │      └── dl_init() si vide
         │
         ├──► bluetooth_init()       // Bluetooth
         ├──► app_init()             // Menu (G1)
         │
         └──► isr_init()             // Interruptions (Timer0)
```

**Ordre important :**
1. **board** en premier (configure les broches)
2. **LCD** ensuite (afficher les erreurs)
3. **Drivers** (I2C, SPI, UART)
4. **Modules** qui utilisent les drivers
5. **ISR** en dernier (active les interruptions)

### Boucle principale : app_main_loop()

```
app_main_loop()
  │
  └──► while(1) {
         │
         ├──► Vérifier g_timer0_flag    // Tick toutes les 10ms
         │      └── g_tick_counter++
         │
         ├──► Mettre à jour la config   // Toutes les 100ms
         │      └── g_sensor_period_ticks = dl_get_sample_period_min() × 100
         │
         ├──► Acquisition capteurs      // Selon période configurée
         │      ├── bmp280_read()
         │      ├── sht30_read()
         │      └── if (dl_is_running())
         │            └── dl_push_record()
         │
         └──► app_loop()                // Menu et boutons (G1)
       }
```

## Types de données partagés

Dans le fichier `core/types.h` il y a des définitions de types partagés entre plusieurs modules.

```c
// Heure et date (RTC)
typedef struct {
  uint8_t hour, min, sec;   // 0-23, 0-59, 0-59
  uint8_t day, month;       // 1-31, 1-12
  uint16_t year;            // 2000-2099
} rtc_time_t;

// Données BMP280
typedef struct {
  int32_t pressure_pa;      // Pression en Pa × 100
  int16_t temp_c_x100;      // Température en °C × 100
} bmp280_data_t;

// Données SHT30
typedef struct {
  uint16_t rh_x100;         // Humidité en % × 100
  int16_t temp_c_x100;      // Température en °C × 100
} sht30_data_t;

// Données complètes (pour affichage et stockage)
typedef struct {
  int16_t  t_c_x100;        // Température
  uint16_t rh_x100;         // Humidité
  int32_t  p_pa;            // Pression
} sensor_data_t;

// Codes d'erreur
typedef enum {
  APP_OK = 0,               // Succès
  APP_EBUS,                 // Erreur bus I2C/SPI
  APP_EDEV,                 // Périphérique non trouvé
  APP_EPARAM,               // Paramètre invalide
  APP_ENCONF,               // Configuration vide
  APP_EFULL                 // Mémoire pleine
} app_err_t;
```

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
