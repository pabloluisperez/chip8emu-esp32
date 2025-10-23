# Comparación de Compilación: SDL3 vs LilyGO

Este documento compara las dos formas de compilar el emulador CHIP-8.

## Tabla Comparativa

| Característica | SDL3 (PC) | LilyGO S3 |
|----------------|-----------|-----------|
| **Build System** | CMake | PlatformIO |
| **Comando de Compilación** | `cmake --build .` | `pio run` |
| **Comando de Ejecución** | `./chip8emu rom.ch8` | `pio run --target upload` |
| **Carga de ROMs** | Desde archivo | Embebida en código |
| **Display** | Ventana 640x320 | TFT 170x320 |
| **Escala por Defecto** | 10x (640x320) | 4x (256x128) |
| **Input** | Teclado QWERTY | Botones GPIO |
| **Audio** | SDL Audio API | Buzzer PWM |
| **Configuración de Teclas** | config/keymap.cfg | Hardcoded en código |
| **Debug** | stdout/stderr | Serial Monitor |
| **Velocidad de Compilación** | ~5-10 segundos | ~30-60 segundos |
| **Tamaño del Binario** | ~2-5 MB | ~1 MB |
| **Hot Reload** | Sí (cambiar ROM) | No (recompilar) |

## Comandos Lado a Lado

### Compilación Inicial

```bash
# SDL3
mkdir build && cd build
cmake ..
cmake --build .

# LilyGO
pio run
```

### Ejecutar/Subir

```bash
# SDL3
./chip8emu roms/pong.ch8

# LilyGO
pio run --target upload
```

### Limpiar Build

```bash
# SDL3
rm -rf build

# LilyGO
pio run --target clean
```

### Ver Logs/Debug

```bash
# SDL3
./chip8emu roms/pong.ch8
# Output directo en la terminal

# LilyGO
pio device monitor
# Output desde el puerto serial
```

## Flujo de Desarrollo Recomendado

### Para desarrollar nuevas características:

1. **Desarrolla en SDL3 primero**
   - Compilación más rápida
   - Debugging más fácil
   - Hot reload de ROMs
   - No necesitas hardware

2. **Prueba en LilyGO después**
   - Verifica que funciona en hardware real
   - Ajusta rendimiento si es necesario
   - Prueba los controles GPIO

### Ejemplo de flujo:

```bash
# 1. Desarrollar en PC con SDL3
cd build
cmake --build .
./chip8emu ../roms/test.ch8
# Hacer cambios, probar rápidamente

# 2. Cuando esté listo, portar a LilyGO
cd ..
# Convertir ROM si es necesario
python tools/rom_to_array.py roms/test.ch8
# Copiar el array a src/main.cpp
pio run --target upload
# Probar en hardware
```

## Archivos Específicos de Plataforma

### SDL3 usa:
```
src/sdl/
  ├── SDLDisplay.cpp
  ├── SDLInput.cpp
  ├── SDLAudio.cpp
  └── SDLPlatform.cpp

include/sdl/
  ├── SDLDisplay.h
  ├── SDLInput.h
  ├── SDLAudio.h
  └── SDLPlatform.h

config/keymap.cfg  ← Configuración de teclas
CMakeLists.txt     ← Build system
```

### LilyGO usa:
```
src/lilygo/
  ├── LilyGoDisplay.cpp
  ├── LilyGoInput.cpp
  ├── LilyGoAudio.cpp
  └── LilyGoPlatform.cpp

include/lilygo/
  ├── LilyGoDisplay.h
  ├── LilyGoInput.h
  ├── LilyGoAudio.h
  └── LilyGoPlatform.h

platformio.ini     ← Build system
```

### Ambos comparten:
```
src/core/
  ├── Chip8.cpp      ← Core del emulador (¡sin cambios!)
  └── Config.cpp

include/core/
  ├── Chip8.h
  └── Config.h

include/interfaces/ ← Interfaces abstractas
  ├── IDisplay.h
  ├── IInput.h
  ├── IAudio.h
  └── IPlatform.h
```

## Cambiar entre Plataformas

El sistema detecta automáticamente la plataforma:

```cpp
// src/main.cpp
#ifdef PLATFORM_LILYGO
    // Código para LilyGO
    #include "../include/lilygo/..."
    using PlatformDisplay = LilyGoDisplay;
    // ...
#else
    // Código para SDL3
    #include "../include/sdl/..."
    using PlatformDisplay = SDLDisplay;
    // ...
#endif
```

### El flag `PLATFORM_LILYGO` se define:

- **CMake**: No se define → Compila para SDL3
- **PlatformIO**: Se define en `platformio.ini` → Compila para LilyGO

```ini
# platformio.ini
build_flags =
    -D PLATFORM_LILYGO  ← Define el flag
```

## Ventajas y Desventajas

### SDL3 (PC)

**Ventajas:**
- ✅ Desarrollo rápido
- ✅ Fácil debugging
- ✅ Cambiar ROMs sin recompilar
- ✅ Display más grande
- ✅ No requiere hardware especial

**Desventajas:**
- ❌ Requiere SDL3 instalado
- ❌ No es portable
- ❌ Consume más recursos

### LilyGO S3

**Ventajas:**
- ✅ Dispositivo portable
- ✅ Bajo consumo
- ✅ Hardware dedicado
- ✅ Pantalla integrada

**Desventajas:**
- ❌ Compilación más lenta
- ❌ ROMs embebidas (no hot reload)
- ❌ Debugging más complejo
- ❌ Requiere hardware físico

## Casos de Uso

### Usa SDL3 si:
- Estás desarrollando nuevas características
- Quieres probar muchas ROMs diferentes
- No tienes un LilyGO disponible
- Necesitas debugging detallado

### Usa LilyGO si:
- Quieres un dispositivo portable
- El proyecto está listo para producción
- Quieres mostrar el emulador en acción
- Te gusta el hardware embebido

## Performance

### SDL3:
- **FPS**: ~60 (limitado por software)
- **Ciclos**: ~600 Hz (10 ciclos × 60 FPS)
- **Latencia**: Baja (~16ms)

### LilyGO S3:
- **FPS**: ~60 (limitado por software)
- **Ciclos**: ~600 Hz (10 ciclos × 60 FPS)
- **Latencia**: Baja (~16ms)
- **Nota**: Puede requerir ajustes según la ROM

## Conclusión

**Recomendación:**
1. Desarrolla y prueba en SDL3
2. Cuando funcione bien, compila para LilyGO
3. Ajusta solo si es necesario

El core es el mismo, así que el comportamiento debería ser idéntico en ambas plataformas. 🎮

