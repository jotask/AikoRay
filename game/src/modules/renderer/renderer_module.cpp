#include "renderer_module.hpp"

#include "modules/renderer/camera.hpp"

namespace raylib
{
    #include <raylib.h>
}

#include <vector>

namespace aiko
{
    RendererModule::~RendererModule()
    {

    }

    void RendererModule::init()
    {
        raylib::SetTargetFPS(60);
    }

    void RendererModule::update()
    {

    }

    void RendererModule::render()
    {

    }

    void RendererModule::beginFrame()
    {
        raylib::BeginDrawing();

        ClearBackground(RAYWHITE);
    }

    void RendererModule::endFrame()
    {
        raylib::EndDrawing();
    }

    void RendererModule::beginScene(Camera* camera, Vector3 position )
    {
        auto cam = raylib::Camera{ position, camera->target, camera->up, camera->fovy, static_cast<int>(camera->projection) };

        static Camera* old = nullptr;

        if ( old != camera )
        {
            old = camera;
            raylib::SetCameraMode(cam, raylib::CAMERA_ORBITAL);  // Set a orbital camera mode   
        }
        else
        {
            raylib::UpdateCamera(&cam);
        }

        raylib::BeginMode3D(cam);

        renderDebug();

    }

    void RendererModule::endScene()
    {
        raylib::EndMode3D();
    }

    void RendererModule::renderDebug()
    {
        raylib::DrawGrid(10, 1.0);

        auto GenMeshCustom = []() {
            raylib::Mesh mesh = { 0 };
            mesh.triangleCount = 1;
            mesh.vertexCount = mesh.triangleCount * 3;
            mesh.vertices = (float*)raylib::MemAlloc(mesh.vertexCount * 3 * sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
            mesh.texcoords = (float*)raylib::MemAlloc(mesh.vertexCount * 2 * sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
            mesh.normals = (float*)raylib::MemAlloc(mesh.vertexCount * 3 * sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

            // Vertex at (0, 0, 0)
            mesh.vertices[0] = 0;
            mesh.vertices[1] = 0;
            mesh.vertices[2] = 0;
            mesh.normals[0] = 0;
            mesh.normals[1] = 1;
            mesh.normals[2] = 0;
            mesh.texcoords[0] = 0;
            mesh.texcoords[1] = 0;

            // Vertex at (1, 0, 2)
            mesh.vertices[3] = 1;
            mesh.vertices[4] = 0;
            mesh.vertices[5] = 2;
            mesh.normals[3] = 0;
            mesh.normals[4] = 1;
            mesh.normals[5] = 0;
            mesh.texcoords[2] = 0.5f;
            mesh.texcoords[3] = 1.0f;

            // Vertex at (2, 0, 0)
            mesh.vertices[6] = 2;
            mesh.vertices[7] = 0;
            mesh.vertices[8] = 0;
            mesh.normals[6] = 0;
            mesh.normals[7] = 1;
            mesh.normals[8] = 0;
            mesh.texcoords[4] = 1;
            mesh.texcoords[5] = 0;

            // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
            raylib::UploadMesh(&mesh, false);

            return mesh;
        };

        auto GenTextureCustom = []() {
            raylib::Image checked = raylib::GenImageChecked(2, 2, 1, 1, RED, GREEN);
            raylib::Texture2D texture = raylib::LoadTextureFromImage(checked);
            raylib::UnloadImage(checked);
            return texture;
        };

        static std::vector<raylib::Texture> textures = {
            GenTextureCustom()
        };
        static int currentTextureIndex = textures.size() - 1;

        static std::vector<raylib::Model> models = {
            raylib::LoadModelFromMesh(raylib::GenMeshPlane(2, 2, 5, 5)),
            raylib::LoadModelFromMesh(raylib::GenMeshCube(2.0f, 1.0f, 2.0f)),
            raylib::LoadModelFromMesh(raylib::GenMeshSphere(2, 32, 32)),
            raylib::LoadModelFromMesh(raylib::GenMeshHemiSphere(2, 16, 16)),
            raylib::LoadModelFromMesh(raylib::GenMeshCylinder(1, 2, 16)),
            raylib::LoadModelFromMesh(raylib::GenMeshTorus(0.25f, 4.0f, 16, 32)),
            raylib::LoadModelFromMesh(raylib::GenMeshKnot(1.0f, 2.0f, 16, 128)),
            raylib::LoadModelFromMesh(raylib::GenMeshPoly(5, 2.0f)),
            raylib::LoadModelFromMesh(GenMeshCustom())

        };
        static int currentModelIndex = models.size() - 1;

        for (size_t i = 0; i < models.size(); i++)
        {
            models[i].materials[0].maps[raylib::MATERIAL_MAP_DIFFUSE].texture = textures[currentTextureIndex];
        }

        if (raylib::IsMouseButtonPressed(raylib::MOUSE_BUTTON_LEFT))
        {
            currentModelIndex = (currentModelIndex + 1) % models.size(); // Cycle between the textures
        }

        static Vector3 position = { 0.0f };

        raylib::DrawModel(models[currentModelIndex], position, 1.0f, MAGENTA);

    }

}