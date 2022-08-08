#pragma once

#include "asset.hpp"

namespace raylib
{
    #include <raylib.h>
}

namespace aiko
{

    class FontAsset : public Asset
    {
    public:
        FontAsset(const std::string path) : Asset(path) { };
        virtual ~FontAsset();
        virtual bool load() override;
    private:
        raylib::Font m_font = { 0 };
    };

    class MusicAsset : public Asset
    {
    public:
        MusicAsset(const std::string path) : Asset(path) { };
        virtual ~MusicAsset();
        virtual bool load() override;

        void setMusicvolumen(float volumen);
        void play();
        void update();

    private:
        raylib::Music m_music = { 0 };
    };

    class SoundAsset : public Asset
    {
    public:
        SoundAsset(const std::string path) : Asset(path) { };
        virtual ~SoundAsset();
        virtual bool load() override;
        void playOneShoot();
    private:
        raylib::Sound m_sound = { 0 };
    };

    class ShaderAsset : public Asset
    {

    private:
        static const std::string combineBothFileIntoOnePath(const std::string vertexShader, const std::string fragmentShader)
        {
            const std::string path =  vertexShader.substr(0, vertexShader.find_last_of("\\/"));
            const std::string::size_type p1(vertexShader.find_last_of('.'));
            const std::string vertex_shader_without_extension = vertexShader.substr(0, p1);
            const std::string::size_type p2(fragmentShader.find_last_of('.'));
            const std::string fragment_shader_without_extension = fragmentShader.substr(0, p2);
            return path + "" + vertex_shader_without_extension + "" + fragment_shader_without_extension;
        };

    public:
        ShaderAsset(const std::string vertexShader, const std::string fragmentShader ) : Asset( combineBothFileIntoOnePath(vertexShader, fragmentShader) ) { };
        virtual ~ShaderAsset();
        virtual bool load() override;
    private:
        const std::string vertexShader;
        const std::string fragmentShader;

        raylib::Shader m_shader = { 0 };

    };

}