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

void SoundEngine::LoadSound(const char* filePath, ALuint& buffer)
{
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(sfinfo));

    // Öffne die Sounddatei
    sndfile = sf_open(filePath, SFM_READ, &sfinfo);
    if (!sndfile) {
        // Fehler beim Öffnen der Datei
        return;
    }

    // Lese die Sounddaten
    ALsizei size = static_cast<ALsizei>(sfinfo.frames) * sfinfo.channels;
    ALsizei freq = sfinfo.samplerate;

    short* membuf = new short[size];
    sf_readf_short(sndfile, membuf, size);

    // Schließe die Sounddatei
    sf_close(sndfile);

    // Erzeuge einen OpenAL-Buffer und kopiere die Daten
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, membuf, size * sizeof(short), freq);

    delete[] membuf;
}

void SoundEngine::PlaySound(const char* filePath) {
    ALuint buffer;
    LoadSound(filePath, buffer);

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
