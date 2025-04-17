#include "Player.hpp"
#include <iostream>
#include "constants.hpp"

const float IMG_WIDTH = 64.0f;
const float IMG_HEIGHT = 64.0f;

const float TEXTURE_WIDTH = 640.0f;
const float TEXTURE_HEIGHT = 128.0f;

glm::vec4 getUVs(int index) {
    // Top Row 0 = leftmost char, 9 = rightmost
    // Bottom Row 0 to 5

    float uvW = IMG_WIDTH / TEXTURE_WIDTH;
    float uvH = IMG_HEIGHT / TEXTURE_HEIGHT;

    float uvX;
    float uvY;
    if (index < 10) {
        // top row
        uvX = index * uvW;
        uvY = 1.0f - uvH;
    } else {
        //bottom row
        uvX = (index - 10) * uvW;
        uvY = 0.0f;
    }
    return glm::vec4(uvX, uvY, uvW, uvH);
}

void Player::init(b2World* world,const glm::vec2& position, const glm::vec2& drawDims,
                  const glm::vec2& collisionDims, ColorRGBA8 color) {
    m_drawDims = drawDims;
    m_color = color;
    m_texture = ResourceManager::getTexture(GE_Images::NINJA);
    m_capsule.init(world, position, collisionDims, 1.0f, 0.1f, true);
    m_position = position;
}

void Player::draw(SpriteBatch& spriteBatch) {

    b2Body* body = m_capsule.getBody();
    destRect.x = body->GetPosition().x - m_drawDims.x / 2.0f;
    destRect.y = body->GetPosition().y - m_capsule.getDimensions().y / 2.0f;
    destRect.z = m_drawDims.x;
    destRect.w = m_drawDims.y;

    m_animSpeed = 0.01f;

    animation(body);

    m_animTime += m_animSpeed;
    if (m_animTime > m_numTiles) {
        m_isPunching = false;
    }

    m_tileIndex = m_tileIndex + (int)m_animTime % m_numTiles;

    m_uvRect = getUVs(m_tileIndex);
    if (m_direction == -1) {
        flippedUV = { m_uvRect.x + m_uvRect.z, m_uvRect.y, -m_uvRect.z, m_uvRect.w };
    } else {
        flippedUV = m_uvRect;
    }

    spriteBatch.draw(destRect, flippedUV, m_texture.id, 0.0f, m_color, body->GetAngle());

}
void Player::animation(b2Body* body) {
    glm::vec2 velocity;
    velocity.x = body->GetLinearVelocity().x;
    velocity.y = body->GetLinearVelocity().y;

    if (m_onGround) {
        if (m_isPunching) {
            m_tileIndex = 11;
            m_numTiles = 4;
            if (m_moveState != PlayerMoveState::PUNCHING) {
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0.0f;
            }
        } else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && m_direction > 0) || (velocity.x < 0 && m_direction < 0))) {
            //Running
            m_tileIndex = 0;
            m_numTiles = 6;
            m_animSpeed = abs(velocity.x) * 0.025f;
            if (m_moveState != PlayerMoveState::RUNNING) {
                m_moveState = PlayerMoveState::RUNNING;
                m_animTime = 0.0f;
            }
        } else {
            //Standing
            m_tileIndex = 10;
            m_numTiles = 1;
            m_moveState = PlayerMoveState::STANDING;
        }
    } else {
        //In Air
        if (m_isPunching) {
            //Kicking in air
            m_tileIndex = 8;
            m_numTiles = 1;
            m_animSpeed *= 0.2f;
            if (m_moveState != PlayerMoveState::PUNCHING) {
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0.0f;
            }
        } else if (abs(velocity.x) > 10.0f) {
            m_tileIndex = 0;
            m_numTiles = 1;
            m_moveState = PlayerMoveState::IN_AIR;
        } else if (velocity.y <= 0.0f) {
            //Falling
            m_tileIndex = 7;
            m_numTiles = 1;
            m_moveState = PlayerMoveState::IN_AIR;
        } else {
            //Rising
            m_tileIndex = 6;
            m_numTiles = 1;
            m_moveState = PlayerMoveState::IN_AIR;
        }
    }
}

void Player::update(Input& inputManager) {
    b2Body* body = m_capsule.getBody();

    if (inputManager.isKeyDown(SDLK_RIGHT)) {
        body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
        m_direction = 1;
    } else if (inputManager.isKeyDown(SDLK_LEFT)) {
        body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
        m_direction = -1;
    }
    if (inputManager.isKeyPressed(SDLK_SPACE)) {
        m_isPunching = true;
    } else if (!inputManager.isKeyPressed(SDLK_SPACE)) {
        m_isPunching = false;
    }
    // Set Max X Velocity
    float MAX_SPEED = 10.0f;
    if (body->GetLinearVelocity().x < -MAX_SPEED) {
        body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
    } else if (body->GetLinearVelocity().x > MAX_SPEED) {
        body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
    }

    // Loop Thru Contact Points
    m_onGround = false;
    for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {
        b2Contact* c = ce->contact;
        if (c->IsTouching()) {
            b2WorldManifold manifold;
            c->GetWorldManifold(&manifold);
            // Check If The Points Are Below
            bool below = false;
            for (int i = 0; i < b2_maxManifoldPoints; i++) {
                if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2 + 0.01f) {
                    below = true;
                    break;
                }
            }
            if (below) {
                m_onGround = true;
                if (inputManager.isKeyDown(SDLK_UP)) {
                    body->ApplyLinearImpulse(b2Vec2(0.0f, 20.0f), b2Vec2(0.0f, 0.0f), true);
                    break;
                }
            }
        }
    }
}
