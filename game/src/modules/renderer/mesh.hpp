#pragma once

#include "utils/raylib_include.hpp"
#include "core/math.hpp"
#include "modules/renderer/shader.hpp"

#include <string>

namespace aiko
{
    class Mesh
    {
    public:
        Mesh();

    private:

        friend class RendererModule;

        raylib::Mesh    m_mesh;
        raylib::Model   m_model;

    };

}
