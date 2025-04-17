#include "AudioEngine.hpp"

/*    Currently, these flags are:

    MIX_INIT_FLAC   = 0x00000001,
    MIX_INIT_MOD    = 0x00000002,
    MIX_INIT_MP3    = 0x00000008,
    MIX_INIT_OGG    = 0x00000010,
    MIX_INIT_MID    = 0x00000020,
    MIX_INIT_OPUS   = 0x00000040,
    MIX_INIT_WAVPACK= 0x00000080

    int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
    int Mix_PlayChannel(int channel, Mix_Chunk *chunk, int loops);
    int Mix_PlayMusic(Mix_Music *music, int loops);
    int Mix_HaltMusic(void);
    void Mix_ResumeMusic(void);
    int Mix_AllocateChannels(int numchans);

    #define MIX_DEFAULT_FREQUENCY   44100
    #define MIX_DEFAULT_FORMAT      AUDIO_S16SYS
    #define MIX_DEFAULT_CHANNELS    2
    #define MIX_MAX_VOLUME          SDL_MIX_MAXVOLUME
    SDL2_mixer/Mix_VolumeChunk
    SDL2_mixer/Mix_VolumeMusic
*/
void SoundEffect::play(int loops) {
    m_effectChannel = Mix_PlayChannel(-1, m_chunk, loops);
    if (m_effectChannel == -1) {
        if (Mix_PlayChannel(0, m_chunk, loops)) {
            std::cout << m_audioChannels << std::endl;
            fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
        }
    }
    if (Mix_VolumeChunk(m_chunk, -1) > 60) {
        Mix_VolumeChunk(m_chunk, 60);
    }
}

void Music::play(int loops) {
    m_musicChannel = Mix_PlayMusic(m_music, loops);
    if (m_musicChannel == -1) {
        fatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
    }
    if (Mix_VolumeMusic(-1) > 60) {
        Mix_VolumeMusic(60);
    }
}
void Music::pause() {
    Mix_PauseMusic();
}
void Music::stop() {
    Mix_HaltMusic();
}
void Music::resume() {
    Mix_ResumeMusic();
}

AudioEngine::AudioEngine() {
    ///Empty
}

AudioEngine::~AudioEngine() {
    destroy();
}

void AudioEngine::init() {
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
        fatalError("Mix_Init Error: " + std::string(Mix_GetError()));
    }
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fatalError("Mix_OpenAudio Error: " + std::string(Mix_GetError()));
    }
    m_isAudioReady = true;
}

void AudioEngine::destroy() {
    if (m_isAudioReady) {
        m_isAudioReady = false;
        for (auto& it : m_effectMap) {
            Mix_FreeChunk(it.second);
        }
        for (auto& it : m_musicMap) {
            Mix_FreeMusic(it.second);
        }
        m_musicMap.clear();
        m_effectMap.clear();
        Mix_CloseAudio();

        Mix_Quit();
    }
}
// std::map<std::string, Mix_Chunk*> m_effectMap;

SoundEffect AudioEngine::loadSoundEffect(std::string filePath) {

    SoundEffect effect;

    auto it = m_effectMap.find(filePath);

    if (it == m_effectMap.end()) {
        // load effect
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (chunk == nullptr) {
            fatalError("Mix_LoadWAV Error: " + std::string(Mix_GetError()));
        }
        effect.m_chunk = chunk;
        m_effectMap[filePath] = chunk;
    } else {
        // already loaded
        effect.m_chunk = it->second;
    }
    return effect;
}


//     std::map<std::string, Mix_Music*> m_musicMap;
Music AudioEngine::loadMusic(std::string filePath) {
    auto it = m_musicMap.find(filePath);
    Music music;

    if (it == m_musicMap.end()) {
        // load music
        Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
        if (mixMusic == nullptr) {
            fatalError("Mix_LoadMUS Error: " + std::string(Mix_GetError()));
        }
        music.m_music = mixMusic;
        m_musicMap[filePath] = mixMusic;

    } else {
        // already loaded
        music.m_music = it->second;

    }
    return music;
}
