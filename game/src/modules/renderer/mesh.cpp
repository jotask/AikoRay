#include "mesh.hpp"

#include "core/platform_constants.hpp"

namespace aiko
{
    Mesh::Mesh()
    {
        m_mesh = raylib::GenMeshPlane(10.0f, 10.0f, 3, 3);
        m_model = raylib::LoadModelFromMesh(m_mesh);
    }

    void Mesh::setShader(Shader shader)
    {
        // Assign out lighting shader to model
        m_model.materials[0].shader = shader.m_shader;
    }

}