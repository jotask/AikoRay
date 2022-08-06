#include "display_module.hpp"

#include "raylib.h"

namespace aiko
{
    DisplayModule::~DisplayModule()
    {
        CloseWindow();
    }

    void DisplayModule::init()
    {
        InitWindow(ScreenWidth, ScreenHeight, "raylib game template");
    }

    void DisplayModule::update()
    {

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