#pragma once

#include "core/math.hpp"
#include "modules/renderer/shader.hpp"

namespace aiko
{
    class Light
    {
    public:
        enum class LightType { DIRECTIONAL, POINT };
    public:

        Light(const int nLight, Shader shader, LightType type, Vector3 position, Vector3 target, Color color, float attenuation);

    // TODO
    private:

        void update();

        friend class RendererModule;

    public:

        int enabledLoc = -1;
        int typeLoc = -1;
        int positionLoc = -1;
        int targetLoc = -1;
        int colorLoc = -1;

        Shader shader;
        bool enabled;
        LightType type;
        Vector3 position;
        Vector3 target;
        Color color;
        float attenuation;

    };

}
