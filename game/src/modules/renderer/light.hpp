#pragma once

#include "core/math.hpp"
#include "modules/assets/assets.hpp"

namespace aiko
{
    class Light
    {
    public:
        enum class LightType { DIRECTIONAL, POINT };
    public:

        Light()
            : enabled(true)
            , type(LightType::POINT)
            , position({ 0.0f })
            , target({0.0f})
            , color({255})
            , attenuation(0.5f)
        {

        }

        Light(LightType type, Vector3 position, Vector3 target, Color color, float attenuation )
            : enabled(true)
            , type(type)
            , target(target)
            , color(color)
            , attenuation(attenuation)
        {

        }

    // TODO
    private:

        friend class RendererModule;

        void createLight();

    public:


        bool enabled;
        LightType type;
        Vector3 position;
        Vector3 target;
        Color color;
        float attenuation;

    };

}
