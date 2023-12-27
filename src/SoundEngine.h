#pragma once

#include <AL/al.h>
#include <AL/alc.h>

class SoundEngine {
public:
    SoundEngine();
    ~SoundEngine();

    void PlaySound(const char* filePath);

private:
    ALCdevice* device;
    ALCcontext* context;
};
