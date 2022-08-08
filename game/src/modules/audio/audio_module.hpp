#pragma once

#include "modules/module.hpp"

#include "modules/assets/assets.hpp"

namespace aiko
{

    class AssetModule;

    class AudioModule : public Module
    {
    public:

        AudioModule() = default;
        virtual ~AudioModule();

        virtual bool connect(ModuleConnector& moduleConnector) override;

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    private:

        AssetModule* assetModule;

        MusicAsset* music;
        SoundAsset* sound;

    };

}