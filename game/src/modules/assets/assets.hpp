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

}