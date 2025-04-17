#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Human.hpp"
#include "Input.hpp"
#include "Camera.hpp"
#include "Weapon.hpp"

class MainGame;
class Player : public Human {

public:
    Player();
    void init(float speed, glm::vec2 pos, Input* input, Camera* camera, std::vector<Bullet>* bullets);
    virtual void update(const std::vector<std::string>& _levelData,
                        std::vector<std::unique_ptr<Human>>& humans,
                        std::vector<std::unique_ptr<Zombie>>& zombies, float deltaTime);
    void addGun(std::unique_ptr<Gun> gun);

private:
    Input* m_input;

    std::vector<std::unique_ptr<Gun>> m_guns;
    int m_currentGunIndex;
    Camera* m_camera;
    std::vector<Bullet>* m_bullets;

};

#endif //PLAYER_HPP
