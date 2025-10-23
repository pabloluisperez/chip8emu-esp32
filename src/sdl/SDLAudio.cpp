#include "../../include/sdl/SDLAudio.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SDLAudio::SDLAudio()
    : audioStream(nullptr), isPlaying(false) {
}

SDLAudio::~SDLAudio() {
    shutdown();
}

bool SDLAudio::init() {
    SDL_AudioSpec spec;
    spec.freq = SAMPLE_RATE;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 1;
    
    audioStream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        nullptr,
        nullptr
    );
    
    if (!audioStream) {
        std::cerr << "Error al crear audio stream SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void SDLAudio::shutdown() {
    if (audioStream) {
        SDL_DestroyAudioStream(audioStream);
        audioStream = nullptr;
    }
}

void SDLAudio::playBeep() {
    if (!audioStream || isPlaying) {
        return;
    }
    
    isPlaying = true;
    
    // Generar onda cuadrada
    const int bufferSize = SAMPLE_RATE / 60; // Un frame a 60 Hz
    int16_t buffer[bufferSize];
    
    for (int i = 0; i < bufferSize; ++i) {
        double time = static_cast<double>(i) / SAMPLE_RATE;
        double value = std::sin(2.0 * M_PI * FREQUENCY * time);
        buffer[i] = static_cast<int16_t>(value * AMPLITUDE);
    }
    
    SDL_PutAudioStreamData(audioStream, buffer, bufferSize * sizeof(int16_t));
    SDL_ResumeAudioStreamDevice(audioStream);
}

void SDLAudio::stopBeep() {
    if (!audioStream || !isPlaying) {
        return;
    }
    
    isPlaying = false;
    SDL_PauseAudioStreamDevice(audioStream);
    SDL_ClearAudioStream(audioStream);
}

