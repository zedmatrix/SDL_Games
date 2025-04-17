#include "MainGame.hpp"
#include "Grid.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include "constants.hpp"

// Some helpful constants.
const float DESIRED_FPS = 60.0f;
const int MAX_PHYSICS_STEPS = 6;
const float MS_PER_SECOND = 1000;
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
const float MAX_DELTA_TIME = 1.0f;

MainGame::~MainGame() {
    // Empty
}

void MainGame::run() {
    init();
    initBalls();

    Uint32 previousTicks = SDL_GetTicks();

    while (m_gameState == GameState::RUNNING) {
        m_fpsLimiter.begin();
        processInput();

        Uint32 newTicks = SDL_GetTicks();
        Uint32 frameTime = newTicks - previousTicks;
        previousTicks = newTicks;

        float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

        int i = 0;

        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

            update(deltaTime);
            totalDeltaTime -= deltaTime;
            i++;
        }
        m_camera.update();
        draw();
        m_fps = m_fpsLimiter.end();
    }
}

void MainGame::init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_screenWidth = SCREEN_WIDTH; /*= 1024;*/
    m_screenHeight = SCREEN_HEIGHT; /*= 768;*/

    m_window.create("Ball Game", m_screenWidth, m_screenHeight, 0);
    m_camera.init(m_screenWidth, m_screenHeight);
    m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));
    
    m_spriteBatch.init();

    // Compile our texture shader
    m_textureProgram.compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();

    m_fpsLimiter.setMaxFPS(60.0f);

    initRenderers();
}

void MainGame::initRenderers() {
    m_ballRenderers.push_back(std::make_unique<BallRenderer>());
    m_ballRenderers.push_back(std::make_unique<MomentumBallRenderer>());
    m_ballRenderers.push_back(std::make_unique<VelocityBallRenderer>(m_screenWidth, m_screenHeight));
    m_ballRenderers.push_back(std::make_unique<TrippyBallRenderer>(m_screenWidth, m_screenHeight));
}

struct BallSpawn {
    BallSpawn(const ColorRGBA8& colr,
              float rad, float m, float minSpeed,
              float maxSpeed, float prob) :
              color(colr),
              radius(rad),
              mass(m),
              randSpeed(minSpeed, maxSpeed),
              probability(prob) {
        // Empty
    }
    ColorRGBA8 color;
    float radius;
    float mass;
    std::uniform_real_distribution<float> randSpeed;
    float probability;
};

void MainGame::initBalls() {
    m_grid = std::make_unique<Grid>(m_screenWidth, m_screenHeight, CELL_SIZE);

#define ADD_BALL(p, ...) \
    totalProbability += p; \
    possibleBalls.emplace_back(__VA_ARGS__);

    // Number of balls to spawn
    const int NUM_BALLS = 1000;

    // Random engine stuff
    std::mt19937 randomEngine((unsigned int)time(nullptr));
    std::uniform_real_distribution<float> randX(0.0f, (float)m_screenWidth);
    std::uniform_real_distribution<float> randY(0.0f, (float)m_screenHeight);
    std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    // Add all possible balls
    std::vector <BallSpawn> possibleBalls;
    float totalProbability = 0.0f;

    /// Random values for ball types
    std::uniform_real_distribution<float> r1(2.0f, 8.0f);
    std::uniform_int_distribution<int> r2(0, 255);

    // Adds the balls using a macro
    ADD_BALL(1.0f, ColorRGBA8(255, 255, 255, 255),
             30.0f, 4.0f, 0.0f, 0.0f, totalProbability);
    // Make a bunch of random ball types
    for (int i = 0; i < NUM_BALLS; i++) {
        ADD_BALL(1.0f, ColorRGBA8(r2(randomEngine), r2(randomEngine), r2(randomEngine), 255),
                 r1(randomEngine), r1(randomEngine), 0.0f, 0.0f, totalProbability);
    }
    // Random probability for ball spawn
    std::uniform_real_distribution<float> spawn(0.0f, totalProbability);

    // Small optimization that sets the size of the internal array to prevent
    // extra allocations.
    m_balls.reserve(NUM_BALLS);

    // Set up ball to spawn with default value
    BallSpawn* ballToSpawn = &possibleBalls[0];
    for (int i = 0; i < NUM_BALLS; i++) {
        // Get the ball spawn roll
        float spawnVal = spawn(randomEngine);
        // Figure out which ball we picked
        for (size_t j = 0; j < possibleBalls.size(); j++) {
            if (spawnVal <= possibleBalls[j].probability) {
                ballToSpawn = &possibleBalls[j];
                break;
            }
        }

        // Get random starting position
        glm::vec2 pos(randX(randomEngine), randY(randomEngine));

        // Hacky way to get a random direction
        glm::vec2 direction(randDir(randomEngine), randDir(randomEngine));
        if (direction.x != 0.0f || direction.y != 0.0f) { // The chances of direction == 0 are astronomically low
            direction = glm::normalize(direction);
        } else {
            direction = glm::vec2(1.0f, 0.0f); // default direction
        }

        // Add ball
        m_balls.emplace_back(ballToSpawn->radius, ballToSpawn->mass, pos, direction * ballToSpawn->randSpeed(randomEngine),
                             ResourceManager::getTexture(GE_Images::CIRCLE).id,
                             ballToSpawn->color);
        // Add the ball do the grid. IF YOU EVER CALL EMPLACE BACK AFTER INIT BALLS, m_grid will have DANGLING POINTERS!
        m_grid->addBall(&m_balls.back());
    }
}

void MainGame::update(float deltaTime) {
    m_ballController.updateBalls(m_balls, m_grid.get(), deltaTime, m_screenWidth, m_screenHeight);
}

void MainGame::draw() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

    m_ballRenderers[m_currentRenderer]->renderBalls(m_spriteBatch, m_balls, projectionMatrix);

    m_textureProgram.use();

    // Make sure the shader uses texture 0
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_textureProgram.unuse();

    m_window.swapBuffer();
}

void MainGame::processInput() {
    // Update input manager
    m_inputManager.update();

    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_ballController.onMouseMove(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
                m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_ballController.onMouseDown(m_balls, (float)evnt.button.x, (float)m_screenHeight - (float)evnt.button.y);
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_ballController.onMouseUp(m_balls);
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

    if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
        m_gameState = GameState::EXIT;
    }
    // Handle gravity changes
    if (m_inputManager.isKeyPressed(SDLK_LEFT)) {
        m_ballController.setGravityDirection(GravityDirection::LEFT);
    } else if (m_inputManager.isKeyPressed(SDLK_RIGHT)) {
        m_ballController.setGravityDirection(GravityDirection::RIGHT);
    } else if (m_inputManager.isKeyPressed(SDLK_UP)) {
        m_ballController.setGravityDirection(GravityDirection::UP);
    } else if (m_inputManager.isKeyPressed(SDLK_DOWN)) {
        m_ballController.setGravityDirection(GravityDirection::DOWN);
    } else if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
        m_ballController.setGravityDirection(GravityDirection::NONE);
    }

    // Switch renderers
    if (m_inputManager.isKeyPressed(SDLK_1)) {
        m_currentRenderer++;
        if (m_currentRenderer >= (int)m_ballRenderers.size()) {
            m_currentRenderer = 0;
        }
    }
}
