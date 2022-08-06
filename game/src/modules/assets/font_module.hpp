#pragma once

#include "modules/module.hpp"

#include <raylib.h>

namespace aiko
{

    class FontModule : public Module
    {
    public:

        FontModule() = default;
        virtual ~FontModule();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

    private:

        Font m_font = { 0 };

    };

}