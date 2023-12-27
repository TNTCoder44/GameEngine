#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.h>

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

class SoundEngine {
public:
    SoundEngine();
    ~SoundEngine();

    void AsyncPlay(const char* filePath);

protected:
    void LoadSound(const char* filePath);
    void PlaySound(const char* filePath);

private:
    ALCdevice* device;
    ALCcontext* context;

    ALuint buffer;
    ALuint source;
    
};
