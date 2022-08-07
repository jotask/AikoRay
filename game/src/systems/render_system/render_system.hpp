#pragma once

#include <stack>

#include "systems/system.hpp"

namespace aiko
{

    class Camera;
    class RendererModule;
    class TransformComponent;

    class RenderSystem: public System
    {
    public:

        RenderSystem();
        virtual ~RenderSystem() = default;

        virtual bool connect(ModuleConnector& moduleConnector, SystemConnector& systemConnector) override;

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        void beginScene(Camera* camera, TransformComponent* tranform);
        void endScene();

    private:

        RendererModule* rendererModule;

    };

}
