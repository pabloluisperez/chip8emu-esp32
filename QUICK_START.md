# Guía Rápida de Inicio

Esta guía te ayudará a compilar y ejecutar el emulador CHIP-8 en ambas plataformas.

## 🖥️ Opción 1: Compilar para PC (SDL3)

### Requisitos

- CMake 3.15+
- Compilador C++17
- SDL3

### Pasos

```bash
# 1. Instalar SDL3 (si no lo tienes)
# Windows: Descargar desde https://github.com/libsdl-org/SDL/releases
# Linux: sudo apt install libsdl3-dev (o compilar desde fuente)
# macOS: brew install sdl3

# 2. Clonar o navegar al directorio del proyecto
cd chip8emu

# 3. Crear y entrar al directorio de build
mkdir build
cd build

# 4. Configurar con CMake
cmake ..

# 5. Compilar
cmake --build .

# 6. Ejecutar (Linux/Mac)
./chip8emu ../roms/IBM_Logo.ch8

# 6. Ejecutar (Windows)
.\chip8emu.exe ..\roms\IBM_Logo.ch8
```

### Controles (PC)

```
Layout CHIP-8:     Teclado:
  1 2 3 C            1 2 3 4
  4 5 6 D            Q W E R
  7 8 9 E            A S D F
  A 0 B F            Z X C V

ESC = Salir
```

### ROMs disponibles

```bash
./chip8emu roms/IBM_Logo.ch8    # Test clásico
./chip8emu roms/pong.ch8        # Juego Pong
./chip8emu roms/test_opcode.ch8 # Test de opcodes
```

---

## 📱 Opción 2: Compilar para LilyGO S3 Display

### Requisitos

- Python 3.7+
- PlatformIO
- Cable USB-C
- LilyGO T-Display S3

### Pasos

```bash
# 1. Instalar PlatformIO
pip install platformio

# 2. Navegar al directorio del proyecto
cd chip8emu

# 3. Compilar
pio run

# 4. Conectar la placa LilyGO con USB-C

# 5. Subir a la placa
pio run --target upload

# 6. Ver el monitor serial (opcional)
pio device monitor
```

### ROM por Defecto

El firmware viene con la ROM **IBM Logo** embebida. Para cambiar a otra ROM:

```bash
# 1. Convertir ROM a array de bytes
python tools/rom_to_array.py roms/pong.ch8

# 2. Copiar el output y reemplazar ROM_DATA en src/main.cpp

# 3. Recompilar y subir
pio run --target upload
```

### Configurar Botones/Buzzer

Edita los pines GPIO en:
- **Botones**: `src/lilygo/LilyGoInput.cpp`
- **Buzzer**: `src/main.cpp` (línea con `LilyGoAudio audio(25)`)

Ver `LILYGO_SETUP.md` para detalles completos.

---

## 🔧 Solución de Problemas

### SDL3: "SDL3 not found"

```bash
# Linux
sudo apt install libsdl3-dev

# Si no está disponible, compila desde fuente:
git clone https://github.com/libsdl-org/SDL
cd SDL
cmake -B build
cmake --build build
sudo cmake --install build
```

### SDL3: La ventana no se abre

- Verifica que tienes drivers de video actualizados
- En Windows, asegúrate de que el ejecutable está en la carpeta `build`

### LilyGO: Error al subir

```bash
# 1. Verifica que la placa esté conectada
pio device list

# 2. Mantén presionado BOOT mientras conectas USB

# 3. Si persiste, especifica el puerto:
pio run --target upload --upload-port COM3  # Windows
pio run --target upload --upload-port /dev/ttyUSB0  # Linux
```

### LilyGO: Display en blanco

- Verifica que la placa sea un T-Display S3
- Los pines del display están en `platformio.ini`
- Revisa el monitor serial para mensajes de error

### LilyGO: Sin sonido

- Conecta un buzzer pasivo al GPIO 25 (o el que hayas configurado)
- Buzzer positivo → GPIO 25
- Buzzer negativo → GND

---

## 📚 Documentación

- **README.md** - Documentación completa del proyecto
- **PORTING_GUIDE.md** - Guía para portar a otras plataformas
- **LILYGO_SETUP.md** - Configuración detallada de hardware LilyGO
- **tools/rom_to_array.py** - Utilidad para convertir ROMs

---

## 🎮 Descargar más ROMs

Puedes descargar ROMs de CHIP-8 desde:
- https://github.com/kripod/chip8-roms
- https://www.zophar.net/pdroms/chip8.html
- https://johnearnest.github.io/chip8Archive/

**Importante:** Respeta las licencias de las ROMs.

---

## 🚀 Estructura del Proyecto

```
chip8emu/
├── src/
│   ├── core/          # Core del emulador (independiente)
│   ├── sdl/           # Implementación SDL3
│   ├── lilygo/        # Implementación LilyGO
│   └── main.cpp       # Punto de entrada (multi-plataforma)
├── include/           # Headers
├── config/            # Configuración de teclas
├── roms/              # ROMs de ejemplo
├── tools/             # Utilidades
├── CMakeLists.txt     # Build system para SDL3
└── platformio.ini     # Build system para LilyGO
```

---

## ✅ Checklist Rápido

### Para SDL3:
- [ ] SDL3 instalado
- [ ] CMake configurado
- [ ] Proyecto compilado
- [ ] ROM ejecutándose

### Para LilyGO:
- [ ] PlatformIO instalado
- [ ] Placa conectada
- [ ] Firmware subido
- [ ] Display funcionando

---

## 💡 Consejos

1. **SDL3**: Usa ROMs con gráficos interesantes como Pong o Space Invaders
2. **LilyGO**: La ROM IBM Logo es perfecta para verificar que todo funciona
3. **Desarrollo**: Prueba en SDL3 primero, luego porta a LilyGO
4. **Debug**: Usa el monitor serial en LilyGO para ver mensajes

---

## 🤝 Contribuir

Si encuentras bugs o mejoras:
1. Reporta issues en el repositorio
2. Las contribuciones son bienvenidas
3. Sigue la arquitectura existente

---

**¡Disfruta tu emulador CHIP-8!** 🎉

