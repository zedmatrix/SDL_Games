#include "Player.hpp"

Player::Player() : m_input(nullptr), m_currentGunIndex(-1) {
    //Empty
}

void Player::init(float speed, glm::vec2 pos, Input* input, Camera* camera, std::vector<Bullet>* bullets) {
    m_speed = speed;
    m_position = pos;
    m_input = input;
    m_camera = camera;
    m_bullets = bullets;
    m_color = ColorRGBA8(255, 64, 64, 255);
    m_health = 300;
}

void Player::addGun(std::unique_ptr<Gun> gun) {
    m_guns.push_back(std::move(gun));
    if (m_currentGunIndex == -1) {
        m_currentGunIndex = 0;
    }
}

void Player::update(const std::vector<std::string>& m_levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies, float deltaTime) {

    if (m_input->isKeyDown(SDLK_UP)) m_position.y += m_speed * deltaTime;
    if (m_input->isKeyDown(SDLK_DOWN)) m_position.y -= m_speed * deltaTime;
    if (m_input->isKeyDown(SDLK_RIGHT)) m_position.x += m_speed * deltaTime;
    if (m_input->isKeyDown(SDLK_LEFT)) m_position.x -= m_speed * deltaTime;

    if (m_input->isKeyPressed(SDLK_1) && m_guns.size() >= 0) {
        m_currentGunIndex = 0;
    } else if (m_input->isKeyPressed(SDLK_2) && m_guns.size() >= 1) {
        m_currentGunIndex = 1;
    }

    if (m_currentGunIndex != -1) {

        glm::vec2 mouseCoords = m_input->getMouseCoords();
        mouseCoords = m_camera->getWorldCoords(mouseCoords);

        glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);

        glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

        m_guns[m_currentGunIndex]->update(m_input->isKeyDown(SDL_BUTTON_LEFT),
                                        centerPosition,
                                        direction,
                                        *m_bullets,
                                        deltaTime);
    }
    collideWithLevel(m_levelData);

}
