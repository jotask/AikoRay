#include "renderer_module.hpp"

#include "modules/assets/asset_module.hpp"
#include "modules/renderer/camera.hpp"
#include "utils/raylib_include.hpp"
#include "core/raylib/raylight.hpp"

#include "core/platform_constants.hpp"
#include "modules/assets/assets.hpp"

#include "modules/renderer/light.hpp"

#include <vector>

namespace aiko
{
    RendererModule::~RendererModule()
    {

    }

    bool RendererModule::connect(ModuleConnector& moduleConnector)
    {
        m_assetModule = moduleConnector.findModule<AssetModule>();
        return true;
    }

    void RendererModule::init()
    {
        raylib::SetTargetFPS(60);

        m_shader.emplace_back(raylib::TextFormat("resources/shaders/glsl%i/base_lighting.vs", GLSL_VERSION), raylib::TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

        for (auto& shader : m_shader)
        {
            shader.m_shader.locs[raylib::SHADER_LOC_VECTOR_VIEW] = shader.GetLocation("viewPos");
            shader.m_shader.locs[raylib::SHADER_LOC_MATRIX_MODEL] = shader.GetLocation("matModel");

            shader.SetValue("ambient", { 0.1f, 0.1f, 0.1f, 1.0f }, Shader::ShaderUniformType::SHADER_UNIFORM_VEC4);
        }

        // FIXME Replace to model pointers, so the reallocation dont' move things around
        m_models.reserve(2);

        m_models.emplace_back(raylib::LoadModelFromMesh(raylib::GenMeshPlane(10.0f, 10.0f, 3, 3)));
        m_models.emplace_back( raylib::LoadModelFromMesh(raylib::GenMeshCube(2.0f, 4.0f, 2.0f)));

        for (auto& shader : m_shader)
        {
            for (auto& model : m_models)
            {
                model.model.materials[0].shader = shader.m_shader;
            }
        }

        m_lights.push_back({ 0, &m_shader[0], Light::LightType::POINT, { -2, 0.5f, -2 }, {0.0f}, raylib::YELLOW, 0.5f });
        m_lights.push_back({ 1, &m_shader[0], Light::LightType::POINT, {  2, 0.5f,  2 }, {0.0f}, raylib::RED, 0.5f });
        m_lights.push_back({ 2, &m_shader[0], Light::LightType::POINT, { -2, 0.5f,  2 }, {0.0f}, raylib::GREEN, 0.5f });
        m_lights.push_back({ 3, &m_shader[0], Light::LightType::POINT, {  2, 0.5f, -2 }, {0.0f}, raylib::BLUE, 0.5f });

    }

    void RendererModule::update()
    {
        // Check key inputs to enable/disable lights
        if (raylib::IsKeyPressed(raylib::KEY_Y)) { m_lights[0].enabled = !m_lights[0].enabled; }
        if (raylib::IsKeyPressed(raylib::KEY_R)) { m_lights[1].enabled = !m_lights[1].enabled; }
        if (raylib::IsKeyPressed(raylib::KEY_G)) { m_lights[2].enabled = !m_lights[2].enabled; }
        if (raylib::IsKeyPressed(raylib::KEY_B)) { m_lights[3].enabled = !m_lights[3].enabled; }
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
        // auto cam = raylib::Camera{ position, camera->target, camera->up, camera->fovy, static_cast<int>(camera->projection) };
        auto cam = raylib::Camera{ {2.0f, 4.0f, 6.0f}, { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, static_cast<int>(camera->projection) };

        static Camera* old = nullptr;

        if ( old != camera )
        {
            old = camera;
            raylib::SetCameraMode(cam, raylib::CAMERA_ORBITAL);  // Set a orbital camera mode
        }
        
        raylib::UpdateCamera(&cam);
        
        float cameraPos[3] = { cam.position.x, cam.position.y, cam.position.z };
        for (auto& shader : m_shader)
        {
            shader.SetValueV(shader.m_shader.locs[raylib::SHADER_LOC_VECTOR_VIEW], cameraPos, Shader::ShaderUniformType::SHADER_UNIFORM_VEC3);
        }

        for (auto& light : m_lights)
        {
            light.update();
        }

        raylib::BeginMode3D(cam);

        // renderDebug();

        renderShader();

    }

    void RendererModule::endScene()
    {
        raylib::EndMode3D();
    }

    void RendererModule::renderShader()
    {

        for (auto& model : m_models)
        {
            model.render();
        }

        for (auto& light : m_lights)
        {
            if ( light.enabled == true )
            {
                raylib::DrawSphereEx(light.position, 0.2f, 8, 8, light.color);
            }
            else
            {
                raylib::DrawSphereWires(light.position, 0.2f, 8, 8, raylib::ColorAlpha(light.color, 0.3f));
            }
        }


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
        static size_t currentTextureIndex = textures.size() - 1;

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