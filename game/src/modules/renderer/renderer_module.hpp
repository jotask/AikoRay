#pragma once

#include "modules/module.hpp"
#include "core/math.hpp"
#include "modules/renderer/light.hpp"
#include "modules/renderer/shader.hpp"
#include "modules/renderer/model.hpp"

#include <vector>

namespace aiko
{

    class Camera;

    class AssetModule;

    class RendererModule : public Module
    {
    public:

        RendererModule() = default;
        virtual ~RendererModule();

        virtual bool connect(ModuleConnector& moduleConnector) override;
        virtual void init() override;

        virtual void update() override;
        virtual void render() override;

        virtual void beginFrame() override;
        virtual void endFrame() override;

        void beginScene(Camera* camera, Vector3 position);
        void endScene();

    private:

        using Lights = std::vector<Light>;
        Lights      m_lights;

        using Shaders = std::vector<Shader>;
        Shaders     m_shader;

        using Models = std::vector<Model>;
        Models     m_models;

        AssetModule* m_assetModule;

        void renderDebug();
        void renderShader();

    };

}