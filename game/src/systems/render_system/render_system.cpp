#include "render_system.hpp"

#include "systems/scene_system/scene.hpp"
#include "modules/renderer/camera.hpp"
#include "modules/renderer/renderer_module.hpp"
#include "components/transform_component.hpp"

namespace aiko
{

    RenderSystem::RenderSystem()
    {

    }

    bool RenderSystem::connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector)
    {
        rendererModule = moduleConnector.findModule<RendererModule>();
        return true;
    }

    void RenderSystem::init()
    {
    }

    void RenderSystem::update()
    {
    }

    void RenderSystem::render()
    {
    }

    void RenderSystem::beginScene(Camera* camera, TransformComponent* tranform)
    {
        rendererModule->beginScene(camera, tranform->position);
    }

    void RenderSystem::endScene()
    {
        rendererModule->endScene();
    }

}
