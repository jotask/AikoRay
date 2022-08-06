#pragma once

#include "module.hpp"

#include "raylib.h"

namespace aiko
{

    class AudioModule : public Module
    {
    public:

        AudioModule() = default;
        virtual ~AudioModule();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    private:

        Music music = { 0 };
        Sound fxCoin = { 0 };

    };

}