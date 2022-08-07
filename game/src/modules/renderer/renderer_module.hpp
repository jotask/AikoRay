#pragma once

#include "modules/module.hpp"
#include "core/math.hpp"

namespace aiko
{

    class Camera;

    class RendererModule : public Module
    {
    public:

        RendererModule() = default;
        virtual ~RendererModule();

        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

        void beginScene(Camera* camera, Vector3 position);
        void endScene();

    };

}