#include "InitData.h"

#ifndef Sound_h
#define Sound_h

// ========================================= TODO Refactor

uint64_t _soundPreviousMillis = 0;
int8_t _soundIndex = -1;
uint16_t _soundTmpShared = 0;

INLINE void SoundInit(){
    pinMode(BUZZER, OUTPUT);
}
void SoundStop(){
    _soundIndex = -1;
    _soundTmpShared = 0;
    noTone(BUZZER);
}

void SoundStart(int8_t index){
    SoundStop();
    _soundIndex = index;
    _soundPreviousMillis = millis();
}

void Sound_1_2(uint64_t millis, bool type){
    if(_soundTmpShared >= 20){
        SoundStop();
        return;
    }
    if(_soundPreviousMillis - millis > 100){
        _soundTmpShared++;
        tone(BUZZER, 520 + (_soundTmpShared * (type ? 10 : -5)));
        _soundPreviousMillis = millis;
    }
}

void SoundTick(uint64_t millis){
    switch (_soundIndex)
    {
    case 0:
        Sound_1_2(millis, true);
        return;
    case 1:
        Sound_1_2(millis, false);
        return;
    default:
        return;
    }
}

INLINE void SoundTick(){
    SoundTick(millis());
}

#endif