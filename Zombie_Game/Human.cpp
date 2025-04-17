#define GLM_ENABLE_EXPERIMENTAL
#include "Human.hpp"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Human::Human() {

}

void Human::init(float speed, glm::vec2 pos) {

    std::mt19937 randomEngine(time(nullptr));
    std::uniform_real_distribution<float> randDir(-2.0f, 2.0f);
    m_color = ColorRGBA8(64, 64, 64, 255);
    m_health = 300;
    m_speed = speed;
    m_position = pos;
    m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    if (m_direction.length() == 0) m_direction = glm::vec2(1.0, 0.0f);

    m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& m_levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies, float deltaTime) {

    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randRotate(-10.0f, 10.0f);

    m_position += m_direction * m_speed * deltaTime;
    if (collideWithLevel(m_levelData) ) {
        m_direction = glm::rotate(m_direction, randRotate(randomEngine));
    }
}
