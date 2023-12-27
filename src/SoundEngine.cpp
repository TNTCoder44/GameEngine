#include "SoundEngine.h"

SoundEngine::SoundEngine() 
{
    device = alcOpenDevice(nullptr);
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
}

SoundEngine::~SoundEngine() 
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void SoundEngine::PlaySound(const char* filePath) {
    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALuint source;
    alGenSources(1, &source);

    // Verknüpfe die Soundquelle mit dem Puffer
    alSourcei(source, AL_BUFFER, buffer);

    // Starte die Wiedergabe
    alSourcePlay(source);

    // Aufräumen
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}
