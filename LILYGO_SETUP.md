# Configuración de Hardware para LilyGO T-Display S3

Este documento explica cómo configurar el hardware del LilyGO T-Display S3 para el emulador CHIP-8.

## Especificaciones del LilyGO T-Display S3

- **MCU**: ESP32-S3
- **Display**: ST7789 170x320 TFT
- **Conectividad**: WiFi, Bluetooth
- **Pines disponibles**: Varios GPIO para expansión
- **Botones integrados**: BOOT (GPIO 0), otros según modelo

## Configuración del Display

El display está preconfigurado en `platformio.ini`:

```ini
-D ST7789_DRIVER=1
-D TFT_WIDTH=170
-D TFT_HEIGHT=320
-D TFT_MOSI=11
-D TFT_SCLK=12
-D TFT_CS=10
-D TFT_DC=13
-D TFT_RST=9
-D TFT_BL=38
```

El emulador CHIP-8 renderiza en 64x32 píxeles con escala de 4x (256x128).

## Configuración de Input (Botones)

### Botones Integrados

El LilyGO T-Display S3 tiene un botón BOOT en GPIO 0:

```cpp
// En src/lilygo/LilyGoInput.cpp
gpioMapping[0x0] = 0;  // Botón BOOT = Tecla 0 de CHIP-8
```

### Añadir Botones Externos

Puedes conectar botones externos a cualquier GPIO disponible. Ejemplo de configuración:

```cpp
void LilyGoInput::setupGPIO() {
    // Botones integrados
    gpioMapping[0x0] = 0;   // BOOT
    
    // Botones externos (ejemplo)
    gpioMapping[0x1] = 1;   // GPIO 1
    gpioMapping[0x2] = 2;   // GPIO 2
    gpioMapping[0x3] = 3;   // GPIO 3
    gpioMapping[0x4] = 4;   // GPIO 4
    gpioMapping[0x5] = 5;   // GPIO 5
    gpioMapping[0x6] = 6;   // GPIO 6
    gpioMapping[0x7] = 7;   // GPIO 7
    gpioMapping[0x8] = 8;   // GPIO 8
    gpioMapping[0x9] = 15;  // GPIO 15
    gpioMapping[0xA] = 16;  // GPIO 16
    gpioMapping[0xB] = 17;  // GPIO 17
    gpioMapping[0xC] = 18;  // GPIO 18
    gpioMapping[0xD] = 19;  // GPIO 19
    gpioMapping[0xE] = 20;  // GPIO 20
    gpioMapping[0xF] = 21;  // GPIO 21
    
    // Configurar todos como INPUT_PULLUP
    for (int pin : gpioMapping) {
        if (pin >= 0) {
            pinMode(pin, INPUT_PULLUP);
        }
    }
}
```

### Esquema de Conexión Recomendado

Para un teclado hexadecimal completo (16 teclas), conecta botones según este layout:

```
    CHIP-8 Layout:        Pins GPIO Sugeridos:
    1  2  3  C            1   2   3   18
    4  5  6  D            4   5   6   19
    7  8  9  E            7   8   15  20
    A  0  B  F            16  0   17  21
```

Cada botón debe conectarse entre un GPIO y GND (con pull-up interno).

## Configuración de Audio (Buzzer)

### Buzzer Pasivo

Si tienes un buzzer pasivo, conéctalo a un GPIO con PWM:

```cpp
// En src/main.cpp (sección PLATFORM_LILYGO)
static LilyGoAudio audio(25);  // GPIO 25 para buzzer
```

**Conexión:**
- Pin positivo del buzzer → GPIO 25
- Pin negativo del buzzer → GND

### Buzzer Activo

Si usas un buzzer activo, necesitarás modificar `src/lilygo/LilyGoAudio.cpp`:

```cpp
void LilyGoAudio::playBeep() {
    if (!isPlaying) {
        digitalWrite(buzzerPin, HIGH);  // Activar buzzer
        isPlaying = true;
    }
}

void LilyGoAudio::stopBeep() {
    if (isPlaying) {
        digitalWrite(buzzerPin, LOW);  // Desactivar buzzer
        isPlaying = false;
    }
}
```

## Pines GPIO Disponibles en LilyGO T-Display S3

Consulta la documentación oficial de tu modelo específico, pero típicamente:

- **GPIO 0**: BOOT button (integrado)
- **GPIO 1-21**: Disponibles para uso general
- **GPIO 9-14**: Display (no usar)
- **GPIO 38**: Backlight del display (no usar)
- **GPIO 43, 44**: UART (evitar si usas Serial)

## Alimentación

- **USB-C**: 5V, ideal para desarrollo
- **Batería**: Conector JST para batería Li-Po (opcional)

## Monitoreo Serial

Para ver mensajes de debug:

```bash
pio device monitor
```

El emulador imprime:
- Estado de inicialización
- Información de ROM cargada
- Mensajes de error si hay problemas

## Cambiar la ROM Embebida

1. Convierte tu ROM a array de bytes:
   ```bash
   python tools/rom_to_array.py roms/pong.ch8
   ```

2. Copia el output al `src/main.cpp` en la sección `PLATFORM_LILYGO`

3. Recompila y sube:
   ```bash
   pio run --target upload
   ```

## Optimización de Rendimiento

### Ajustar Velocidad del Emulador

En `src/main.cpp` (sección PLATFORM_LILYGO):

```cpp
const int CYCLES_PER_FRAME = 10;  // Reducir si va muy rápido
```

### Ajustar Escala del Display

En `src/main.cpp`:

```cpp
static LilyGoDisplay display(4);  // Cambiar escala (2-8)
```

Con escala 4: 64×32 → 256×128 (deja espacio en el display 170×320)

## Troubleshooting

### El display no muestra nada

1. Verifica las conexiones del display en `platformio.ini`
2. Comprueba que el backlight esté encendido
3. Revisa el monitor serial para errores

### Los botones no responden

1. Verifica que los pines GPIO están bien mapeados
2. Usa pull-up interno o externo
3. Comprueba continuidad con multímetro

### No hay sonido

1. Verifica la conexión del buzzer
2. Comprueba que el GPIO tiene capacidad PWM
3. Prueba con otro GPIO

### El emulador va muy lento

1. Aumenta `CYCLES_PER_FRAME`
2. Reduce la escala del display
3. Compila en modo Release (no Debug)

### Error al compilar

1. Actualiza PlatformIO: `pio upgrade`
2. Limpia el proyecto: `pio run --target clean`
3. Verifica que TFT_eSPI esté instalado

## Recursos Adicionales

- [Documentación LilyGO](http://www.lilygo.cn/)
- [Repositorio TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [CHIP-8 Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [PlatformIO Docs](https://docs.platformio.org/)

## Esquemático de Ejemplo

```
    LilyGO T-Display S3
    ┌─────────────────┐
    │                 │
    │   ┌─────────┐   │
    │   │ ST7789  │   │
    │   │ Display │   │
    │   │ 170x320 │   │
    │   └─────────┘   │
    │                 │
    │  [BOOT]  [RST]  │ ← Botones integrados
    │                 │
    │  GPIO Pines:    │
    │  1,2,3,4,5...   │ ← Para botones externos
    └────[USB-C]──────┘
          ↓
       Alimentación
       + Serial Debug
```

¡Buena suerte con tu emulador CHIP-8 en LilyGO! 🎮

