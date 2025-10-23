#include "../../include/lilygo/LilyGoAudio.h"
#include <Arduino.h>

LilyGoAudio::LilyGoAudio(int buzzerPin) 
    : buzzerPin(buzzerPin), isPlaying(false) {
}

LilyGoAudio::~LilyGoAudio() {
    shutdown();
}

bool LilyGoAudio::init() {
    pinMode(buzzerPin, OUTPUT);
    return true;
}

void LilyGoAudio::shutdown() {
    stopBeep();
}

void LilyGoAudio::playBeep() {
    if (!isPlaying) {
        tone(buzzerPin, FREQUENCY);
        isPlaying = true;
    }
}

void LilyGoAudio::stopBeep() {
    if (isPlaying) {
        noTone(buzzerPin);
        isPlaying = false;
    }
}

