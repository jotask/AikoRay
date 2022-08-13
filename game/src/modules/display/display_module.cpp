#include "display_module.hpp"

#include "utils/raylib_include.hpp"
#include "events/event_system.hpp"
#include "events/engine_events.hpp"

namespace aiko
{
    DisplayModule::~DisplayModule()
    {
        raylib::CloseWindow();
    }

    void DisplayModule::init()
    {
        raylib::SetConfigFlags(raylib::FLAG_WINDOW_RESIZABLE | raylib::FLAG_VSYNC_HINT);
        raylib::InitWindow(ScreenWidth, ScreenHeight, "raylib game template");
    }

    void DisplayModule::update()
    {
        if (raylib::IsWindowResized() && !raylib::IsWindowFullscreen())
        {
            auto width = raylib::GetScreenWidth();
            auto height = raylib::GetScreenHeight();
            EventSystem::it().sendEvent(WindowResizeEvent(width, height));
        }

        if(raylib::WindowShouldClose() == true) // Detect window close button or ESC key
        {
            EventSystem::it().sendEvent(WindowCloseEvent());
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