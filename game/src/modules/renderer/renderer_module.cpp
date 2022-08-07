#include "renderer_module.hpp"

#include "modules/renderer/camera.hpp"

namespace raylib
{
    #include <raylib.h>
}

namespace aiko
{
    RendererModule::~RendererModule()
    {

    }

    void RendererModule::init()
    {
        raylib::SetTargetFPS(60);
    }

    void RendererModule::update()
    {

    }

    void RendererModule::render()
    {

    }

    void RendererModule::beginFrame()
    {
        raylib::BeginDrawing();

        ClearBackground(RAYWHITE);
    }

    void RendererModule::endFrame()
    {
        raylib::EndDrawing();
    }

    void RendererModule::beginScene(Camera* camera, Vector3 position )
    {
        auto cam = raylib::Camera{ position, camera->target, camera->up, camera->fovy, static_cast<int>(camera->projection) };
        raylib::BeginMode3D(cam);
    }

    void RendererModule::endScene()
    {
        raylib::EndMode3D();
    }

}