#include "MainGame.hpp"
#include <iostream>
#include "constants.hpp"

MainGame::MainGame() {
    //Empty
}

void MainGame::run() {
    m_screenWidth = SCREEN_WIDTH;
    m_screenHeight = SCREEN_HEIGHT;
    m_window.create(m_title, m_screenWidth, m_screenHeight, 0);
    m_fpsLimiter.setMaxFPS(60.0f);
    m_isRunning = true;
    init();

    while (m_isRunning) {
        m_fpsLimiter.begin();

        m_inputManager.update();

        update();

        draw();
        if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) m_isRunning = false;

        m_fps = m_fpsLimiter.end();
        m_window.swapBuffer();
    }
}

void MainGame::init() {
    b2Vec2 gravity(0.0f, -15.0f);
    m_world = std::make_unique<b2World>(gravity);

    //Make Ground
    GLTexture groundTexture = ResourceManager::getTexture(GE_Images::STEEL);
    Box groundBox;
    groundBox.init(m_world.get(), glm::vec2(0.0f, -20.0f), glm::vec2(100.0f, 5.0f),
                   groundTexture, ColorRGBA8(255, 255, 255, 255), false, false);
    m_boxes.push_back(groundBox);

    //Make Wall
    GLTexture wallTexture = ResourceManager::getTexture(GE_Images::BRICKS);
    Box wallBox;
    wallBox.init(m_world.get(), glm::vec2(-20.0f, 0.0f), glm::vec2(2.0f, 20.0f),
                   wallTexture, ColorRGBA8(255, 255, 255, 255), false, false);
    m_boxes.push_back(wallBox);

    // Make a bunch of boxes
    std::mt19937 randGenerator(time(NULL));
    std::uniform_real_distribution<float> xPos(-10.0f, 25.0f);
    std::uniform_real_distribution<float> yPos(-10.0f, 25.0f);

    std::uniform_real_distribution<float> size(1.5, 3.5f);
    std::uniform_int_distribution<int> color(50, 255);
    std::uniform_real_distribution<float> angleDist(-2.0f, 2.0f);

    m_texture = ResourceManager::getTexture(GE_Images::WOOD);
    const int NUM_BOXES = 20;

    for (int i = 0; i < NUM_BOXES; i++) {
        ColorRGBA8 randColor;
        randColor.r = color(randGenerator);
        randColor.g = color(randGenerator);
        randColor.b = color(randGenerator);
        randColor.a = 255;
        float s = size(randGenerator);
        Box newBox;
        newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)),
                    glm::vec2(s, s), m_texture, randColor, true, false, angleDist(randGenerator));
        m_boxes.push_back(newBox);
    }

    m_spriteBatch.init();
    // Compile our texture shader
    m_textureProgram.compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
    // Compile our light program
    m_lightProgram.compileShaders(GE_Light::VERTEX, GE_Light::FRAGMENT);
    m_lightProgram.addAttribute("vertexPosition");
    m_lightProgram.addAttribute("vertexColor");
    m_lightProgram.addAttribute("vertexUV");
    m_lightProgram.linkShaders();


    m_camera.init(m_screenWidth, m_screenHeight);
    m_camera.setScale(16.0f);

    // Init Player

    m_player.init(m_world.get(), glm::vec2(0.0f, 20.0f),
                  glm::vec2(2.1f, 3.1f), glm::vec2(2.0f, 3.0f),   //drawDims, collisonDims
                  ColorRGBA8(255, 255, 255, 255));
}

void MainGame::update() {
    m_camera.update();

    checkInput();

    m_player.update(m_inputManager);

    if (m_inputManager.isKeyPressed(SDLK_z)) m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
    if (m_inputManager.isKeyPressed(SDLK_x)) m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
    // Update Physics for Box
    m_world->Step(1.0f / 60.0f, 6, 2);
}

void MainGame::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
    m_textureProgram.use();

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    m_spriteBatch.begin();

    for (auto& b : m_boxes) {
        b.draw(m_spriteBatch);
    }
    m_player.draw(m_spriteBatch);
    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    // Additive Blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Render Some Test Lights
    Light playerLight;
    playerLight.color = ColorRGBA8(255, 128, 255, 200);
    playerLight.position = m_player.getPosition() - glm::vec2(0.0f, -10.0f);
    playerLight.size = 30.0f;

    Light mouseLight;
    mouseLight.color = ColorRGBA8(128, 255, 128, 128);
    mouseLight.position = m_camera.getWorldCoords(m_inputManager.getMouseCoords());
    mouseLight.size = 45.0f;

    m_lightProgram.use();
    pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    m_spriteBatch.begin();

    playerLight.draw(m_spriteBatch);
    mouseLight.draw(m_spriteBatch);

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
    m_lightProgram.unuse();
    // reset to regular alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void MainGame::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {

        switch (evnt.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}
