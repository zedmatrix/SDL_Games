#ifndef BOX2D_HPP
#define BOX2D_HPP

#include <box2d/box2d.h>
#include <glm/glm.hpp>

#include "Vertex.hpp"
#include "SpriteBatch.hpp"
#include "GLTexture.hpp"

struct Light {
    void draw(SpriteBatch& spriteBatch) {
        glm::vec4 destRect;
        destRect.x = position.x - size / 2.0f;
        destRect.y = position.y - size / 2.0f;
        destRect.z = size;
        destRect.w = size;
        spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
    }

    ColorRGBA8 color;
    glm::vec2 position;
    float size;
};

class Capsule {
public:
    Capsule();

    void init(b2World* world,
              const glm::vec2& position,
              const glm::vec2& dimensions,
              float density,
              float friction,
              bool fixedRotation = true);

    b2Body* getBody() const { return m_body; }
    b2Fixture* getFixture(int index) const { return m_fixtures[index]; }
    const glm::vec2& getDimensions() const { return m_dimensions; }

private:
    b2Body* m_body = nullptr;
    b2Fixture* m_fixtures[3];
    glm::vec2 m_dimensions;

};


class Box {

public:
    Box();

    void init(b2World* world,const glm::vec2& position, const glm::vec2& dimensions,
               GLTexture texture, ColorRGBA8 color,
               bool isDynamic,
               bool fixedRotation,
               float angle = 0.0f,
               glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    void draw(SpriteBatch& spriteBatch);
    bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }
    // Test if a point is inside the box
    bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }

    // Beautiful accessors
    b2Body*           getBody()          const { return m_body; }
    b2Fixture*        getFixture()       const { return m_fixture; }
    const glm::vec2&  getDimensions()    const { return m_dimensions; }
    glm::vec2         getPosition()      const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
    glm::vec4         getUvRect()        const { return m_uvRect; }
    const ColorRGBA8& getColor()         const { return m_color; }
    float             getAngle()         const { return m_body->GetAngle(); }
    const GLTexture&  getTexture()       const { return m_texture; }
    // const bool&       getFixedRotation() const { return m_fixedRotation; }
    // const bool&       getIsDynamic()     const { return m_isDynamic; }
private:
    glm::vec4 m_uvRect;
    b2Body* m_body = nullptr;
    b2Fixture* m_fixture = nullptr;
    glm::vec2 m_dimensions;
    ColorRGBA8 m_color;
    GLTexture m_texture;
    bool m_fixedRotation;
    bool m_isDynamic;

};

#endif //BOX2D_HPP
