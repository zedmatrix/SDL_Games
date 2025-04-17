#ifndef MAINGAME_HPP
#define MAINGAME_HPP

#include <box2d/box2d.h>

#include <vector>
#include <memory>
#include "MainWindow.hpp"
#include "Timing.hpp"
#include "Input.hpp"
#include "GLTexture.hpp"
#include "SpriteBatch.hpp"
#include "Camera.hpp"
#include "GLSLProgram.hpp"

#include "Player.hpp"
#include "Box2d.hpp"


class MainGame {

public:
    MainGame();
    ~MainGame() = default;

    void run();

private:
    void update();
    void draw();
    void checkInput();
    void init();

    MainWindow m_window;
    FpsLimiter m_fpsLimiter;
    Input m_inputManager;

    SpriteBatch m_spriteBatch;
    GLSLProgram m_textureProgram;
    GLSLProgram m_lightProgram;
    Camera m_camera;
    GLTexture m_texture;
    std::vector<Box> m_boxes;
    std::unique_ptr<b2World> m_world;

    Player m_player;

    float m_fps = 0.0f;
    bool m_isRunning = false;
    std::string m_title = "Ninja Box2D";
    int m_screenWidth;
    int m_screenHeight;

};

#endif //MAINGAME_HPP
