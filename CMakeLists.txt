cmake_minimum_required(VERSION 3.15)
project(chip8emu VERSION 1.0 LANGUAGES CXX)

# Estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opción para seleccionar la plataforma
option(BUILD_LILYGO "Build for LilyGO platform" OFF)
option(BUILD_SDL_VENDORED "Build SDL3 from vendored libraries" ON)


if(BUILD_LILYGO)
    message(STATUS "Building for LilyGO platform (not fully supported in CMake)")
    message(STATUS "Please use PlatformIO for LilyGO: pio run")
    message(FATAL_ERROR "Use PlatformIO for LilyGO builds")
else()
    message(STATUS "Building for SDL3 platform")
    
    # Buscar SDL3
    if(BUILD_SDL_VENDORED)
        # This assumes you have added SDL as a submodule in vendored/SDL
        message(STATUS "Building SDL3 from vendored source")
        set(SDL_SHARED ON CACHE BOOL "Build shared SDL3 library" FORCE)
        set(SDL_STATIC OFF CACHE BOOL "Don't build static SDL3 library" FORCE)
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "Don't build SDL3 test library" FORCE)
        add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    else()
        find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    endif()
    
    # Archivos fuente para SDL3
    set(SOURCES
        src/main.cpp
        src/core/Chip8.cpp
        src/core/Config.cpp
        src/sdl/SDLDisplay.cpp
        src/sdl/SDLInput.cpp
        src/sdl/SDLAudio.cpp
        src/sdl/SDLPlatform.cpp
    )
endif()

# Crear ejecutable
add_executable(chip8emu ${SOURCES})

# Incluir directorios
target_include_directories(chip8emu PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Enlazar SDL3 (solo para build SDL)
if(NOT BUILD_LILYGO)
    if(BUILD_SDL_VENDORED)
        target_link_libraries(chip8emu PRIVATE SDL3-shared)
    else()
        target_link_libraries(chip8emu PRIVATE SDL3::SDL3)
    endif()
endif()

# Opciones de compilación
if(MSVC)
    target_compile_options(chip8emu PRIVATE /W4)
else()
    target_compile_options(chip8emu PRIVATE -Wall -Wextra -pedantic)
endif()

# Copiar archivos de configuración al directorio de build
file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR})

# Instalación
install(TARGETS chip8emu DESTINATION bin)
install(DIRECTORY config DESTINATION .)
install(DIRECTORY roms DESTINATION .)

