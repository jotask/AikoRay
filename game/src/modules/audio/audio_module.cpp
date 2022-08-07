#include "audio_module.hpp"

#include <raylib.h>

namespace aiko
{

    AudioModule::~AudioModule()
    {
       // Unload global data loaded
        UnloadMusicStream(music);
        UnloadSound(fxCoin);

        raylib::CloseAudioDevice();
    }

    void AudioModule::init()
    {
        raylib::InitAudioDevice();

        music = raylib::LoadMusicStream("resources/ambient.ogg");
        fxCoin = raylib::LoadSound("resources/coin.wav");
        
        SetMusicVolume(music, 1.0f);
        PlayMusicStream(music);

    }

    void AudioModule::update()
    {
        UpdateMusicStream(music);
    }

    void AudioModule::render()
    {

    }

    void AudioModule::beginFrame()
    {

    }

    void AudioModule::endFrame()
    {

    }

}