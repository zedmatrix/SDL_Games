#ifndef MAINGAME_HPP
#define MAINGAME_HPP
/// External Libraries
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <format>
#include <print>
#include <random>
#include <ctime>
#include <cstdio>
#include <algorithm>

/// Internal Includes
#include "MainWindow.hpp"
#include "Error.hpp"
#include "GLSLProgram.hpp"
#include "GLTexture.hpp"
#include "ResourceManager.hpp"
#include "IOManager.hpp"
#include "SpriteBatch.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Zombie.hpp"
#include "Weapon.hpp"
#include "Timing.hpp"
#include "AudioEngine.hpp"

enum class GameState {PLAY, EXIT};

class MainGame {

public:
    MainGame();

    void run();

private:
    void initSystems();
    void initAudio();
    void initLevel();
    void initShaders();
    void gameLoop();
    void processInput();
    void checkVictory();
    void drawGame();
    void updateAgents(float deltaTime);
    void updateBullets(float deltaTime);

    MainWindow m_mainWindow;
    int m_screenWidth;
    int m_screenHeight;
    float m_fps = 0.0f;
    int m_currentLevel = 0;
    int m_numHumansKilled = 0;
    int m_numZombiesKilled = 0;

    GameState m_gameState;
    Camera m_camera;                ///< main camera
    SpriteBatch m_agentSpriteBatch; ///< draws all agents
    GLSLProgram m_colorProgram;     ///< for opengl color program
    Input m_inputManager;
    FpsLimiter m_fpsLimiter;        ///< fps limiter init

    Player* m_player;
    std::vector<std::unique_ptr<Level>> m_levels;
    std::vector<std::unique_ptr<Human>> m_humans;   ///< all humans + player
    std::vector<std::unique_ptr<Zombie>> m_zombies; ///< all zombies
    std::vector<Bullet> m_bullets;

    AudioEngine m_audioEngine;
    Music m_introMusic;
    SoundEffect m_pistol;
    SoundEffect m_rifle;
    SoundEffect m_shotgun;
    SoundEffect m_mp5;

};

#endif //MAINGAME_HPP
