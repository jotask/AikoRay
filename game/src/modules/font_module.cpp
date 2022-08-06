#include "font_module.hpp"

#include "raylib.h"

namespace aiko
{
    FontModule::~FontModule()
    {
        UnloadFont(m_font);
    }

    void FontModule::init()
    {
        m_font = LoadFont("resources/mecha.png");
    }

    void FontModule::update()
    {

    }

    void FontModule::render()
    {

    }

    void FontModule::beginFrame()
    {

    }

    void FontModule::endFrame()
    {

    }

}