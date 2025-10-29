# === CRÉATION DES DOSSIERS ===
$folders = @(
    "app",
    "drivers",
    "modules",
    "core"
)
$folders | ForEach-Object { New-Item -ItemType Directory -Force -Path $_ }

# === LISTE DES FICHIERS ===
$files = @(
    "app/app_main.h", "app/app_main.c",
    "app/app_menu.h", "app/app_menu.c",

    "drivers/i2c_bus.h", "drivers/i2c_bus.c",
    "drivers/spi_bus.h", "drivers/spi_bus.c",
    "drivers/uart_pc.h", "drivers/uart_pc.c",
    "drivers/uart_bt.h", "drivers/uart_bt.c",
    "drivers/lcd.h",

    "modules/rtc_ds1307.h", "modules/rtc_ds1307.c",
    "modules/bmp280.h", "modules/bmp280.c",
    "modules/sht30.h", "modules/sht30.c",
    "modules/eeprom_m93c66.h", "modules/eeprom_m93c66.c",
    "modules/datalogger.h", "modules/datalogger.c",
    "modules/bluetooth_proto.h", "modules/bluetooth_proto.c",

    "core/board.h", "core/board.c",
    "core/isr.h", "core/isr.c",
    "core/types.h"
)

# === CRÉATION DES FICHIERS AVEC EN-TÊTE STANDARD ===
foreach ($f in $files) {
  if (-not (Test-Path $f)) {
    New-Item -ItemType File -Path $f -Force | Out-Null
    $name  = [IO.Path]::GetFileName($f)
    $guard = ($name -replace '\W','_').ToUpper()

    if ($f -like "*.h") {
      $content = @(
        "// ===============================================",
        "// File:   $name",
        "// Desc:   Header file for $name",
        "// ===============================================",
        "",
        "#ifndef $guard",
        "#define $guard",
        "",
        "// prototypes ici",
        "",
        "#endif // $guard"
      )
      $content | Set-Content -Encoding UTF8 $f
    }
    elseif ($f -like "*.c") {
      $hdr = ($name -replace '\.c$','.h')
      $content = @(
        "// ===============================================",
        "// File:   $name",
        "// Desc:   Source file for $name",
        "// ===============================================",
        "",
        "#include `"$hdr`""
      )
      $content | Set-Content -Encoding UTF8 $f
    }
  }
}