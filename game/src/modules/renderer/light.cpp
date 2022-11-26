#include "light.hpp"

#include "utils/raylib_include.hpp"

namespace aiko
{

    Light::Light(const int nLight, Shader* shader, LightType type, Vector3 position, Vector3 target, Color color, float attenuation)
        : shader(shader)
        , enabled(true)
        , position(position)
        , type(type)
        , target(target)
        , color(color)
        , attenuation(attenuation)
    {
       enabledLoc = shader->GetLocation(raylib::TextFormat("lights[%i].enabled", nLight));
       typeLoc = shader->GetLocation(raylib::TextFormat("lights[%i].type", nLight));
       positionLoc = shader->GetLocation(raylib::TextFormat("lights[%i].position", nLight));
       targetLoc = shader->GetLocation(raylib::TextFormat("lights[%i].target", nLight));
       colorLoc = shader->GetLocation(raylib::TextFormat("lights[%i].color", nLight));
    }

    void Light::update()
    {

        // Send to shader light enabled state and type
        shader->SetValue(enabledLoc, enabled);
        shader->SetValue(typeLoc, static_cast<int>( type ) );

        // Send to shader light position values
        Vector3 p = { position.x, position.y, position.z };
        shader->SetValue(positionLoc, p );

        // Send to shader light target position values
        Vector3 t = { target.x, target.y, target.z };
        shader->SetValue(targetLoc, t);

        // Send to shader light color values
        Vector4 c = { (float)color.r / (float)255, (float)color.g / (float)255, (float)color.b / (float)255, (float)color.a / (float)255 };
        shader->SetValue(colorLoc, c);
    }
}