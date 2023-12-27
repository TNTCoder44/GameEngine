#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.h>

#include <string>

class SoundEngine {
public:
    SoundEngine();
    ~SoundEngine();

    void LoadSound(const char* filePath, ALuint& buffer);
    void PlaySound(const char* filePath);

private:
    ALCdevice* device;
    ALCcontext* context;
};
