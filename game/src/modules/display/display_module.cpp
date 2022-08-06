#include "display_module.hpp"

#include <raylib.h>

#include "events/event_system.hpp"
#include "events/engine_events.hpp"

namespace aiko
{
    DisplayModule::~DisplayModule()
    {
        CloseWindow();
    }

    void DisplayModule::init()
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
        InitWindow(ScreenWidth, ScreenHeight, "raylib game template");
    }

    void DisplayModule::update()
    {
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            auto width = GetScreenWidth();
            auto height = GetScreenHeight();
            EventSystem::it().sendEvent(WindowResizeEvent(width, height));
        }
    }

    void DisplayModule::render()
    {

    }

    void DisplayModule::beginFrame()
    {

    }

    void DisplayModule::endFrame()
    {

    }

}