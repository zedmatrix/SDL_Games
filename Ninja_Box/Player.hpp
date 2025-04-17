#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SDL2/SDL.h>
#include "Box2d.hpp"
#include "SpriteBatch.hpp"
#include "GLTexture.hpp"
#include "ResourceManager.hpp"
#include "Input.hpp"

enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

class Player {

public:
    void init(b2World* world,const glm::vec2& position, const glm::vec2& drawDims,
              const glm::vec2& collisionDims, ColorRGBA8 color);

    void draw(SpriteBatch& spriteBatch);
    void update(Input& inputManager);

    const Capsule& getCapsule() const { return m_capsule; }
    glm::vec2 getPosition() const {
        glm::vec2 rv;
        rv.x = m_capsule.getBody()->GetPosition().x;
        rv.y = m_capsule.getBody()->GetPosition().y;
        return rv;
    }

private:
    Capsule m_capsule;
    GLTexture m_texture;
    ColorRGBA8 m_color;
    glm::vec4 m_uvRect;
    glm::vec2 m_drawDims;
    glm::vec2 m_position;
    glm::vec4 flippedUV;
    glm::vec4 destRect;

    PlayerMoveState m_moveState = PlayerMoveState::STANDING;
    void animation(b2Body* body);
    int m_tileIndex;
    int m_numTiles;
    float m_animSpeed = 0.2f;
    float m_animTime = 0.0f;

    int m_direction = 1; // 1 or -1
    bool m_onGround = false;
    bool m_isPunching = false;
};

#endif //PLAYER_HPP
