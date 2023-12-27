#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.h>

#include <string>
#include <iostream>

class SoundEngine {
public:
    SoundEngine();
    ~SoundEngine();

    void LoadSound(const char* filePath);
    void PlaySound(const char* filePath);

private:
    ALCdevice* device;
    ALCcontext* context;

    ALuint buffer;
    ALuint source;
};
