#include "MainGame.hpp"
#include "constants.hpp"

const float HUMAN_SPEED = 2.0f;
const float ZOMBIE_SPEED = 0.8f;
const float PLAYER_SPEED = 8.0f;
const float CAMERA_SCALE = 1.0f / 3.0f;

const float DESIRED_FPS = 60.0f;
const float MS_PER_SECOND = 1000;
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
const int MAX_PHYSICS_STEPS = 6;
const float MAX_DELTA_TIME = 1.0f;

int m_playerHealth = 10000;

MainGame::MainGame () : m_gameState(GameState::PLAY), m_player(nullptr) {
    ///Empty
}

/// Called from main.cpp
void MainGame::run() {
    initSystems();
    initAudio();
    initLevel();
    gameLoop();
}

/// Initialize System
void MainGame::initSystems() {
    // Initialize Everything and Set Up Double Buffering
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    m_screenWidth = SCREEN_WIDTH; /* = 1024;*/
    m_screenHeight = SCREEN_HEIGHT; /*= 768;*/
    m_mainWindow.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
    initShaders();


    m_agentSpriteBatch.init();
    m_camera.init(m_screenWidth, m_screenHeight);
}
/// Initialize Audio
void MainGame::initAudio() {
    m_audioEngine.init();
    m_introMusic = m_audioEngine.loadMusic(GE_Sound::INTRO);
    m_introMusic.play();
    m_pistol = m_audioEngine.loadSoundEffect(GE_Sound::PISTOL);
    m_mp5 = m_audioEngine.loadSoundEffect(GE_Sound::MACHINE_GUN);
    m_rifle = m_audioEngine.loadSoundEffect(GE_Sound::RIFLE);
    m_shotgun = m_audioEngine.loadSoundEffect(GE_Sound::SHOTGUN);

}
/// Initialize Level
void MainGame::initLevel() {
    // TODO add to constants
    m_levels.push_back(std::make_unique<Level>("../level1.txt"));
    // m_currentLevel = 0; // in header

    // Initialize Player
    m_player = new Player();
    m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);
    m_humans.emplace_back(m_player);

    //// Gun(std::string name, int rate, int bps, int damage, float speed, float spread)
    const float BULLET_SPEED = 20.0f;
    m_player->addGun(std::make_unique<Gun>("Magnum", 10, 5, 10.0f, BULLET_SPEED, 5.0f, &m_pistol));
    m_player->addGun(std::make_unique<Gun>("Rifle", 10, 10, 5.0f, BULLET_SPEED, 8.0f, &m_rifle));
    m_player->addGun(std::make_unique<Gun>("Shotgun", 50, 5, 10.0f, BULLET_SPEED, 30.0f, &m_shotgun));
    m_player->addGun(std::make_unique<Gun>("MP5", 5, 50, 10.0f, BULLET_SPEED, 10.0f, &m_mp5));

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> randPosX(5, m_levels[m_currentLevel]->getWidth() - 5);
    std::uniform_int_distribution<int> randPosY(5, m_levels[m_currentLevel]->getHeight() - 5);

    // Human Initialize
    for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
        m_humans.push_back(std::make_unique<Human>());
        glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH, randPosY(randomEngine) * TILE_WIDTH);
        m_humans.back()->init(HUMAN_SPEED, pos);
    }

    // Zombie start position
    const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getStartZombiePos();

    std::cout << "Zombie Start Positions: " << std::endl;
    for (size_t i = 0; i < zombiePositions.size(); i++) {
        m_zombies.push_back(std::make_unique<Zombie>());
        m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }
}

/// Initialize Shader Program
void MainGame::initShaders() {
    m_colorProgram.compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
    m_colorProgram.addAttribute("vertexPosition");
    m_colorProgram.addAttribute("vertexColor");
    m_colorProgram.addAttribute("vertexUV");
    m_colorProgram.linkShaders();

}

void MainGame::gameLoop() {
    // for Frame Step Calculations
    uint32_t previousTicks = SDL_GetTicks();

    m_fpsLimiter.setMaxFPS(DESIRED_FPS);
    m_camera.setScale(CAMERA_SCALE);
    int frameCount = 0;
    while (m_gameState == GameState::PLAY) {
        m_fpsLimiter.begin();
        uint32_t newTicks = SDL_GetTicks();
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        checkVictory();

        m_inputManager.update();

        processInput();

        int i = 0;
        while (totalDeltaTime > 0.0d && i < MAX_PHYSICS_STEPS) {
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            updateAgents(deltaTime);
            updateBullets(deltaTime);
            totalDeltaTime -= deltaTime;
            i++;
        }

        m_camera.setPosition(m_player->getPosition());

        m_camera.update();

        drawGame();

        m_fps = m_fpsLimiter.end();
        frameCount++;
        if (frameCount > 1000) {
            std::println("Frames per Second: {} FPS\n", m_fps);
            frameCount = 0;
        }
    }
}

void MainGame::updateAgents(float deltaTime) {
    /// update all humans
    for (size_t i = 0; i < m_humans.size(); i++) {
        m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
                           m_humans, m_zombies, deltaTime);
    }
    // update zombies
    for (size_t i = 0; i < m_zombies.size(); i++) {
        m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
                           m_humans, m_zombies, deltaTime);
    }
    // Update Human Collisions
    for (size_t i = 0; i < m_humans.size(); i++) {
        for (size_t j = i + 1; j < m_humans.size(); j++) {
            m_humans[i]->collideWithAgent(m_humans[j].get());
        }
    }
    // Update Zombie Collisions
    for (size_t i = 0; i < m_zombies.size(); i++) {
        // Collide with Zombies
        for (size_t j = i + 1; j < m_zombies.size(); j++) {
            m_zombies[i]->collideWithAgent(m_zombies[j].get());
        }
        // Collide with Humans
        for (size_t j = 1; j < m_humans.size(); j++) {

            if (m_zombies[i]->collideWithAgent(m_humans[j].get())) {
                m_zombies.push_back(std::make_unique<Zombie>());
                m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
                m_humans[j] = std::move(m_humans.back());
                m_humans.pop_back();
            }
        }
        if (m_zombies[i]->collideWithAgent(m_player)) {
            std::cout << "Zombie Attacked You: " << m_playerHealth-- << std::endl;
        }
    }
}
void MainGame::updateBullets(float deltaTime) {
    for (size_t i = 0; i < m_bullets.size(); ) {
        if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
            m_bullets[i] = m_bullets.back();
            m_bullets.pop_back();
        } else {
            i++;
        }
    }
    bool wasBulletRemoved;
    // Collide with Humans and Zombies
    for (size_t i = 0; i < m_bullets.size(); i++) {
        wasBulletRemoved = false;
        for (size_t j = 0; j < m_zombies.size(); ) {
            if (m_bullets[i].collideWithAgent(m_zombies[j].get())) {
                //damage zombie

                if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
                    // kill the zombie
                    m_zombies[j] = std::move(m_zombies.back());
                    m_zombies.pop_back();
                    m_numZombiesKilled++;
                } else {
                    j++;
                }
                // remove the bullet
                m_bullets[i] = m_bullets.back();
                m_bullets.pop_back();
                wasBulletRemoved = true;
                i--;
                break;  ///< bullet is gone
            } else {
                j++;
            }
        }
        //Loop thru humans
        if (wasBulletRemoved == false) {
            for (size_t j = 1; j < m_humans.size(); j++) {   ///< skip 0 = player
                if (m_bullets[i].collideWithAgent(m_humans[j].get())) {
                    if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
                        // kill the human
                        m_humans[j] = std::move(m_humans.back());
                        m_humans.pop_back();
                        m_numHumansKilled++;
                    } else {
                        j++;
                    }
                    m_bullets[i] = m_bullets.back();
                    m_bullets.pop_back();

                    wasBulletRemoved = true;
                    break;
                } else {
                    j++;
                }
            }
        }
    }
} //end of updateBullets
void MainGame::checkVictory() {
    /// TODO multiple level change

    if (m_zombies.empty()) {
        std::printf("*** You Win ***\n\t You Killed %d Humans and %d Zombies\n\t There are %ld out of %d humans left",
                    m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());
        m_gameState = GameState::EXIT;
    }
    if (m_humans.size() < 3) {
        std::printf("*** You Loose ***\n\t You Killed %d Humans and %d Zombies\n\t There are %ld Zombies Left\n",
                    m_numHumansKilled, m_numZombiesKilled, m_zombies.size());
        m_gameState = GameState::EXIT;
    }

}

void MainGame::processInput() {
    SDL_Event m_event;
    while (SDL_PollEvent(&m_event)) {
        switch (m_event.type) {
            case SDL_QUIT:
                m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(m_event.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(m_event.button.button);
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(m_event.motion.x, m_event.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(m_event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(m_event.key.keysym.sym);
                break;
        }
        if (m_inputManager.isKeyPressed(SDLK_q)) m_gameState = GameState::EXIT;
    }
}

void MainGame::drawGame() {
    //Set the base depth and clear color and depth buffer
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
    glActiveTexture(GL_TEXTURE0);

    m_colorProgram.use();
    // binding textures

    GLuint textureLocation = m_colorProgram.getUniformLocation("mySampler");
    glUniform1i(textureLocation, 0);

    glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

    GLuint pLocation = m_colorProgram.getUniformLocation("P");

    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    /// Draw the Level Data
    m_levels[m_currentLevel]->draw();

    /// Draw the Humans and Players
    m_agentSpriteBatch.begin();

    /// Player should always be 0 as initialized first
    m_humans[0]->setTexture(GE_Images::PLAYER);
    m_humans[0]->draw(m_agentSpriteBatch);

    const glm::vec2 agentDims(AGENT_RADIUS * 2);

    /// Humans are next in initialization
    for (size_t i = 1; i < m_humans.size(); i++) {
        if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims)) {
            m_humans[i]->setTexture(GE_Images::HUMAN);
            m_humans[i]->draw(m_agentSpriteBatch);
        }
    }

    /// Zombies are next in initialization
    for (size_t i = 0; i < m_zombies.size(); i++) {
        if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims)) {
            m_zombies[i]->setTexture(GE_Images::ZOMBIE);
            m_zombies[i]->draw(m_agentSpriteBatch);
        }
    }

    for (size_t i = 0; i < m_bullets.size(); i++) {
        m_bullets[i].draw(m_agentSpriteBatch);
    }
    m_agentSpriteBatch.end();
    m_agentSpriteBatch.renderBatch();

    // Swap our buffer and unbind opengl
    glBindTexture(GL_TEXTURE_2D, 0);
    m_colorProgram.unuse();
    m_mainWindow.swapBuffer();
}
