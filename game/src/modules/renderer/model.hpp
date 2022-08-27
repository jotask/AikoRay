#pragma once

#include "core/math.hpp"
#include "modules/renderer/shader.hpp"

namespace aiko
{
    class Model
    {
    private:
        friend class RendererModule;

    public:

        Model(raylib::Model m);
        ~Model();

    private:

        void render();

        raylib::Model model;

    };

}
