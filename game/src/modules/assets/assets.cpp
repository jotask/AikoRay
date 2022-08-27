#include "assets.hpp"

#include "core/platform_detection.hpp"

namespace aiko
{

    FontAsset::~FontAsset()
    {
        raylib::UnloadFont(m_font);
    }

    bool FontAsset::load()
    {
        m_font = raylib::LoadFont(m_path.c_str());
        // TODO :: Check if asset has loaded correctly
        return true;
    }

    MusicAsset::~MusicAsset()
    {
        raylib::UnloadMusicStream(m_music);
    }

    bool MusicAsset::load()
    {
        m_music = raylib::LoadMusicStream(m_path.c_str());
        // TODO :: Check if asset has loaded correctly
        return true;
    }

    void MusicAsset::setMusicvolumen(float volumen)
    {
        raylib::SetMusicVolume(m_music, 1.0f);
    }

    void MusicAsset::play()
    {
        raylib::PlayMusicStream(m_music);
    }

    void MusicAsset::update()
    {
        raylib::UpdateMusicStream(m_music);
    }

    SoundAsset::~SoundAsset()
    {
        raylib::UnloadSound(m_sound);
    }

    bool SoundAsset::load()
    {
        m_sound = raylib::LoadSound(m_path.c_str());
        // TODO :: Check if asset has loaded correctly
        return true;
    }

    void SoundAsset::playOneShoot()
    {
        raylib::PlaySound(m_sound);
    }

}