#pragma once

#ifndef _WIN32

#include <AL/al.h>
#include <AL/alc.h>

#include <sndfile.h>

#include <cstring>
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
    void LoadWAV(const char* filePath);
    void LoadSound(const char* filePath);
    void PlaySound(const char* filePath);

    std::string GetFileExtension(const std::string& filePath);

private:
    ALCdevice* device;
    ALCcontext* context;

    ALuint buffer;
    ALuint source;

    ALsizei sampleRate;
    ALsizei channels;
    ALenum encoding;
};

#endif
