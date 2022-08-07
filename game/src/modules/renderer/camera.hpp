#pragma once

#include "core/math.hpp"

namespace aiko
{
    class Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
    public:

        Camera()
            : target({0.0f})
            , up({0.0f, 1.0f, 0.0f})
            , fovy(45.0f)
            , projection(ProjectionType::Perspective)
        {

        }

        Camera( Vector3 target, Vector3 up, float fovy, ProjectionType projection )
            : target(target)
            , up(up)
            , fovy(fovy)
            , projection(projection)
        {

        }

    // TODO
    private:
    public:

        Vector3 target;                 // Camera target it looks-at
        Vector3 up;                     // Camera up vector (rotation over its axis)
        float fovy;                     // Camera field-of-view apperture in Y (degrees) in perspective, used as near plane width in orthographic
        ProjectionType projection;      // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC

    };

}
