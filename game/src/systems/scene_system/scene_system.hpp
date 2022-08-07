#pragma once

#include <stack>

#include "systems/system.hpp"
#include "core/aiko_types.hpp"

namespace aiko
{

    class Scene;
    class RenderSystem;

    class SceneSystem: public System
    {
    public:

        SceneSystem();
        virtual ~SceneSystem() = default;

        virtual bool connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector) override;

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        Scene* getCurrentScene();

    private:
        Ref<Scene> m_currentScene;
        RenderSystem* m_renderer;
    };

}
