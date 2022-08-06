#include "renderer_module.hpp"

#include <raylib.h>

namespace aiko
{
    RendererModule::~RendererModule()
    {

    }

    void RendererModule::init()
    {
        SetTargetFPS(60);
    }

    void RendererModule::update()
    {

    }

    void RendererModule::render()
    {

    }

    void RendererModule::beginFrame()
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
    }

    void RendererModule::endFrame()
    {
        EndDrawing();
    }

}