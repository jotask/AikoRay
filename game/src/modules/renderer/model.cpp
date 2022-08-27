#include "model.hpp"

#include "utils/raylib_include.hpp"

namespace aiko
{

    Model::Model(raylib::Model m)
        : model(m)
    {

    }

    Model::~Model()
    {
        raylib::UnloadModel(model);
    }

    void Model::render()
    {
        raylib::DrawModel(model, { 0.0f }, 1.0f, raylib::WHITE);
    }

}