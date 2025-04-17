#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <random>
#include <ctime>
#include <cmath>
#include <memory>
#include "Camera.hpp"
#include "SpriteBatch.hpp"
#include "Input.hpp"
#include "MainWindow.hpp"
#include "GLSLProgram.hpp"
#include "Timing.hpp"
#include "BallController.hpp"
#include "ResourceManager.hpp"

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame {
public:
    ~MainGame();
    void run();


private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

    int m_currentRenderer = 0;
    std::vector<std::unique_ptr<BallRenderer> > m_ballRenderers;

    BallController m_ballController; ///< Controls balls

    MainWindow m_window; ///< The main window
    SpriteBatch m_spriteBatch; ///< Renders all the balls

    Camera m_camera; ///< Renders the scene
    Input m_inputManager; ///< Handles input
    GLSLProgram m_textureProgram; ///< Shader for textures]

    FpsLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

