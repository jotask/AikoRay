#include "audio_module.hpp"

#include "modules/assets/asset_module.hpp"
#include "utils/raylib_include.hpp"

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

        assetModule->load<MusicAsset>("resources/ambient.ogg");
        assetModule->load<SoundAsset>("resources/coin.wav");

        const auto musicUUID = assetModule->load<MusicAsset>("resources/ambient.ogg");
        const auto soundUUID = assetModule->load<SoundAsset>("resources/coin.wav");
        music = assetModule->getAsset<MusicAsset>(musicUUID);
        sound = assetModule->getAsset<SoundAsset>(soundUUID);

        music->setMusicvolumen(1.0f);
        music->play();

        sound->playOneShoot();

    }

    void AudioModule::update()
    {
        music->update();
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