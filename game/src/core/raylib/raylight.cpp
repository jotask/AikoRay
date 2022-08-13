#include "core/raylib/raylight.hpp"

#ifndef RAYLIGHT_ENABLED

namespace aiko
{

    namespace raylib_helper
    {

        Light CreateLight(int type, Vector3 position, Vector3 target, Color color, raylib::Shader shader)
        {
            Light light = { 0 };

            if (lightsCount < MAX_LIGHTS)
            {
                light.enabled = true;
                light.type = type;
                light.position = position;
                light.target = target;
                light.color = color;

                // NOTE: Lighting shader naming must be the provided ones
                light.enabledLoc = raylib::GetShaderLocation(shader, raylib::TextFormat("lights[%i].enabled", lightsCount));
                light.typeLoc = raylib::GetShaderLocation(shader, raylib::TextFormat("lights[%i].type", lightsCount));
                light.positionLoc = raylib::GetShaderLocation(shader, raylib::TextFormat("lights[%i].position", lightsCount));
                light.targetLoc = raylib::GetShaderLocation(shader, raylib::TextFormat("lights[%i].target", lightsCount));
                light.colorLoc = raylib::GetShaderLocation(shader, raylib::TextFormat("lights[%i].color", lightsCount));

                UpdateLightValues(shader, light);

                lightsCount++;
            }

            return light;

        }

        void UpdateLightValues(raylib::Shader shader, Light light)
        {
            // Send to shader light enabled state and type
            SetShaderValue(shader, light.enabledLoc, &light.enabled, raylib::SHADER_UNIFORM_INT);
            SetShaderValue(shader, light.typeLoc, &light.type, raylib::SHADER_UNIFORM_INT);

            // Send to shader light position values
            float position[3] = { light.position.x, light.position.y, light.position.z };
            SetShaderValue(shader, light.positionLoc, position, raylib::SHADER_UNIFORM_VEC3);

            // Send to shader light target position values
            float target[3] = { light.target.x, light.target.y, light.target.z };
            SetShaderValue(shader, light.targetLoc, target, raylib::SHADER_UNIFORM_VEC3);

            // Send to shader light color values
            float color[4] = { (float)light.color.r / (float)255, (float)light.color.g / (float)255,
                               (float)light.color.b / (float)255, (float)light.color.a / (float)255 };
            SetShaderValue(shader, light.colorLoc, color, raylib::SHADER_UNIFORM_VEC4);
        }

    }

}

#endif
