#include "light.hpp"

#include "utils/raylib_include.hpp"

namespace aiko
{

    Light::Light(const int nLight, Shader shader, LightType type, Vector3 position, Vector3 target, Color color, float attenuation)
        : shader(shader)
        , enabled(true)
        , position(position)
        , type(type)
        , target(target)
        , color(color)
        , attenuation(attenuation)
    {
       enabledLoc = shader.GetLocation(raylib::TextFormat("lights[%i].enabled", nLight));
       typeLoc = shader.GetLocation(raylib::TextFormat("lights[%i].type", nLight));
       positionLoc = shader.GetLocation(raylib::TextFormat("lights[%i].position", nLight));
       targetLoc = shader.GetLocation(raylib::TextFormat("lights[%i].target", nLight));
       colorLoc = shader.GetLocation(raylib::TextFormat("lights[%i].color", nLight));
    }

    void Light::update()
    {

        // Send to shader light enabled state and type
        shader.SetValueV(enabledLoc, &enabled, Shader::ShaderUniformType::SHADER_UNIFORM_INT);
        shader.SetValueV(typeLoc, &type, Shader::ShaderUniformType::SHADER_UNIFORM_INT);

        // Send to shader light position values
        float p[3] = { position.x, position.y, position.z };
        shader.SetValueV(positionLoc, &p, Shader::ShaderUniformType::SHADER_UNIFORM_VEC3);

        // Send to shader light target position values
        float t[3] = { target.x, target.y, target.z };
        shader.SetValueV(targetLoc, &t, Shader::ShaderUniformType::SHADER_UNIFORM_VEC3);

        // Send to shader light color values
        float c[4] = { (float)color.r / (float)255, (float)color.g / (float)255, (float)color.b / (float)255, (float)color.a / (float)255 };
        shader.SetValueV(colorLoc, &c, Shader::ShaderUniformType::SHADER_UNIFORM_VEC4);
    }
}