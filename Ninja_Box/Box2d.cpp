#include "Box2d.hpp"
#include <cstdio>

Capsule::Capsule() {
    //Empty
}
void Capsule::init(b2World* world,
              const glm::vec2& position,
              const glm::vec2& dimensions,
              float density,
              float friction,
              bool fixedRotation) {
    m_dimensions = dimensions;
    // Make the body
    b2BodyDef bodyDef;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = fixedRotation;
    m_body = world->CreateBody(&bodyDef);

    // Create the box
    b2PolygonShape boxShape;
    boxShape.SetAsBox(m_dimensions.x / 2.0f, (m_dimensions.y - m_dimensions.x) / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

    // Create the circles
    b2CircleShape circleShape;
    circleShape.m_radius = dimensions.x / 2.0f;

    b2FixtureDef circleDef;
    circleDef.shape = &circleShape;
    circleDef.density = density;
    circleDef.friction = friction;

    //Bottom circle
    circleShape.m_p.Set(0.0f, (-m_dimensions.y + m_dimensions.x) / 2.0f);
    m_fixtures[1] = m_body->CreateFixture(&circleDef);
    //Top Circle
    circleShape.m_p.Set(0.0f, (m_dimensions.y - m_dimensions.x) / 2.0f);
    m_fixtures[2] = m_body->CreateFixture(&circleDef);

}

Box::Box() {
    //Empty
}

void Box::init(b2World* world,const glm::vec2& position, const glm::vec2& dimensions,
               GLTexture texture, ColorRGBA8 color,
               bool isDynamic,
               bool fixedRotation,
               float angle, /*= 0.0f*/
               glm::vec4 uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/) {
    m_dimensions = dimensions;
    m_color = color;
    m_texture = texture;
    m_uvRect = uvRect;
    m_fixedRotation = fixedRotation;
    m_isDynamic = isDynamic;

    //std::printf("uvRect x(%0.2f) y(%0.2f) z(%0.2f) w(%0.2f)\n", uvRect.x, uvRect.y, uvRect.z, uvRect.w);
    // Make the body
    b2BodyDef bodyDef;

    if (isDynamic) {
        bodyDef.type = b2_dynamicBody;
    } else {
        bodyDef.type = b2_staticBody;
    }
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.angle = angle;
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(m_dimensions.x / 2.0f, m_dimensions.y / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    m_fixture = m_body->CreateFixture(&fixtureDef);
}

// void Box::destroy(b2World* world) {
//     world->DestroyBody(m_body);
// }
//
void Box::draw(SpriteBatch& spriteBatch) {
    glm::vec4 destRect;
    destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
    destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
    destRect.z = m_dimensions.x;
    destRect.w = m_dimensions.y;
    spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());
}
