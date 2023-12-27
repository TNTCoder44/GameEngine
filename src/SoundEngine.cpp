#include "SoundEngine.h"

SoundEngine::SoundEngine()
    : buffer(0), source(0)
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open OpenAL device" << std::endl;
    }

    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        std::cerr << "Failed to create OpenAL context" << std::endl;
        alcCloseDevice(device);
    }

    if (!alcMakeContextCurrent(context))
    {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);
}

SoundEngine::~SoundEngine()
{
    alDeleteBuffers(1, &buffer);
    alDeleteSources(1, &source);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void SoundEngine::LoadSound(const char *filePath)
{
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(sfinfo));

    // Öffne die Sounddatei
    sndfile = sf_open(filePath, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        // Fehler beim Öffnen der Datei
        std::cerr << "Cannot open Soundfile!" << std::endl;
        return;
    }

    // Lese die Sounddaten
    ALsizei size = static_cast<ALsizei>(sfinfo.frames) * sfinfo.channels;
    ALsizei freq = sfinfo.samplerate;

    short *membuf = new short[size];
    sf_readf_short(sndfile, membuf, size);

    // Schließe die Sounddatei
    sf_close(sndfile);

    alBufferData(buffer, AL_FORMAT_MONO16, membuf, size * sizeof(short), freq);

    delete[] membuf;
}

void SoundEngine::PlaySound(const char *filePath)
{
    // Lade den sound
    LoadSound(filePath);

    // Verknüpfe die Soundquelle mit dem Puffer
    alSourcei(source, AL_BUFFER, buffer);

    // Starte die Wiedergabe
    alSourcePlay(source);

    // Warte, bis die Wiedergabe beendet ist
    ALint state;
    do
    {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);
}
