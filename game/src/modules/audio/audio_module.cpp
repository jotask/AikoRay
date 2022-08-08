#include "audio_module.hpp"

#include "modules/assets/asset_module.hpp"

namespace raylib
{
    #include <raylib.h>
}

namespace aiko
{

    AudioModule::~AudioModule()
    {
        raylib::CloseAudioDevice();
    }

    bool AudioModule::connect(ModuleConnector& moduleConnector)
    {
        assetModule = moduleConnector.findModule<AssetModule>();
        return true;
    }

    void AudioModule::init()
    {
        raylib::InitAudioDevice();

        music = assetModule->getAsset<MusicAsset>("resources/ambient.ogg");
        sound = assetModule->getAsset<SoundAsset>("resources/coin.wav");
        
        music->setMusicvolumen(1.0f);
        music->play();

    }

    void AudioModule::update()
    {

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