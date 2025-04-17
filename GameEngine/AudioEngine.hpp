#ifndef AUDIOENGINE_HPP
#define AUDIOENGINE_HPP

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <iostream>
#include "Error.hpp"

class SoundEffect {

public:
    friend class AudioEngine;
    void play(int loops = 0);

private:
    Mix_Chunk* m_chunk = nullptr;
    int m_effectChannel = -1;
    int m_audioChannels = -1;
};

class Music {

public:
    friend class AudioEngine;
    void play(int loops = 0);
    static void pause();
    static void stop();
    static void resume();

private:
    Mix_Music* m_music = nullptr;
    int m_musicChannel = -1;
};

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();
    void init();
    void destroy();

    SoundEffect loadSoundEffect(std::string filePath);
    Music loadMusic(std::string filePath);

private:
    bool m_isAudioReady = false;
    std::map<std::string, Mix_Chunk*> m_effectMap;
    std::map<std::string, Mix_Music*> m_musicMap;
};

#endif //AUDIOENGINE_HPP
