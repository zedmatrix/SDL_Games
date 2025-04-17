#include "BallController.hpp"
#include "constants.hpp"

// Struct Ball
Ball::Ball(float radius, float mass, const glm::vec2& pos,
           const glm::vec2& vel, unsigned int textureId,
           const ColorRGBA8& color) {
    this->radius = radius;
    this->mass = mass;
    this->position = pos;
    this->velocity = vel;
    this->textureId = textureId;
    this->color = color;
}

//Ball Renderer
void BallRenderer::renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                               const glm::mat4& projectionMatrix) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<GLSLProgram>();
        m_program->compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, ball.color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unuse();
}

void MomentumBallRenderer::renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                               const glm::mat4& projectionMatrix) {

    glClearColor(0.2f, 0.0f, 0.2f, 0.5f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<GLSLProgram>();
        m_program->compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        ColorRGBA8 color;
        GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
        color.r = colorVal;
        color.g = colorVal;
        color.b = colorVal;
        color.a = colorVal;
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unuse();
}

VelocityBallRenderer::VelocityBallRenderer(int screenWidth, int screenHeight) :
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight) {
    // Empty
}


void VelocityBallRenderer::renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                                       const glm::mat4& projectionMatrix) {
    glClearColor(0.1f, 0.1f, 0.0f, 0.5f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<GLSLProgram>();
        m_program->compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        ColorRGBA8 color;

        float mult = 100.0f;
        GLubyte colorVal = (GLubyte)(glm::clamp(ball.velocity.x * mult, 0.0f, 255.0f));
        color.r = 128;
        color.g = (GLubyte)((ball.position.x / m_screenWidth) * 255.0f);
        color.b = (GLubyte)((ball.position.y / m_screenHeight) * 255.0f);
        color.a = colorVal;
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unuse();
}

TrippyBallRenderer::TrippyBallRenderer(int screenWidth, int screenHeight) :
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight) {
    // Empty
}

void TrippyBallRenderer::renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
    glClearColor(0.0f, 0.1f, 0.1f, 0.7f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<GLSLProgram>();
        m_program->compileShaders(GE_Shader::VERTEX, GE_Shader::FRAGMENT);
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Change these constants to get cool stuff
    float TIME_SPEED = 0.01f;
    float DIVISOR = 4.0f; // Increase to get more arms
    float SPIRAL_INTENSITY = 10.0f; // Increase to make it spiral more

    m_time += TIME_SPEED;

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        ColorRGBA8 color;

        // Get vector from center point
        glm::vec2 centerVec = ball.position - glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
        float centerDist = glm::length(centerVec);

        // Get angle from the horizontal
        float angle = atan2(centerVec.x, centerVec.y) / (3.1415926f / DIVISOR);
        // Move with time
        angle -= m_time;
        // Add the spiral
        angle += (centerDist / m_screenWidth) * SPIRAL_INTENSITY;

        color.r = (GLubyte)(angle * 255.0f);
        color.g = (GLubyte)(angle * 255.0f * cos(m_time));
        color.b = (GLubyte)(angle * 255.0f * sin(m_time));
        color.a = (GLubyte)(glm::clamp(1.0f - (centerDist / (m_screenWidth / 2.0f)), 0.0f, 1.0f) * 255.0f);
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unuse();
}
//Ball Controller
void BallController::updateBalls(std::vector <Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY) {
    const float FRICTION = 0.01f;
    // Update our grabbed balls velocity
    if (m_grabbedBall != -1) {
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
    }

    glm::vec2 gravity = getGravityAccel();

    for (size_t i = 0; i < balls.size(); i++) {
        // get handle for less typing
        Ball& ball = balls[i];
        // Update motion if its not grabbed
        if ((int)i != m_grabbedBall) {
            ball.position += ball.velocity * deltaTime;
            // Apply friction
            glm::vec2 momentumVec = ball.velocity * ball.mass;
            if (momentumVec.x != 0 || momentumVec.y != 0) {
                if (FRICTION < glm::length(momentumVec)) {
                    ball.velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / ball.mass;
                } else {
                    ball.velocity = glm::vec2(0.0f);
                }
            }
            // Apply gravity
            ball.velocity += gravity * deltaTime;
        }
        // Check wall collision
        if (ball.position.x < ball.radius) {
            ball.position.x = ball.radius;
            if (ball.velocity.x < 0) ball.velocity.x *= -1;
        } else if (ball.position.x + ball.radius >= maxX) {
            ball.position.x = maxX - ball.radius - 1;
            if (ball.velocity.x > 0) ball.velocity.x *= -1;
        }
        if (ball.position.y < ball.radius) {
            ball.position.y = ball.radius;
            if (ball.velocity.y < 0) ball.velocity.y *= -1;
        } else if (ball.position.y + ball.radius >= maxY) {
            ball.position.y = maxY - ball.radius - 1;
            if (ball.velocity.y > 0) ball.velocity.y *= -1;
        }

        // Check to see if the ball moved
        Cell* newCell = grid->getCell(ball.position);
        if (newCell != ball.ownerCell) {
            grid->removeBallFromCell(&balls[i]);
            grid->addBall(&balls[i], newCell);
        }
    }
    // Updates all collisions using the spatial partitioning
    updateCollision(grid);

    //// Update our grabbed ball
    if (m_grabbedBall != -1) {
        // Update the velocity again, in case it got changed by collision
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
        m_prevPos = balls[m_grabbedBall].position;
    }
}

void BallController::onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY) {
    for (size_t i = 0; i < balls.size(); i++) {
        // Check if the mouse is hovering over a ball
        if (isMouseOnBall(balls[i], mouseX, mouseY)) {
            m_grabbedBall = i; // BE AWARE, if you change the order of the balls in the vector, this becomes invalid!
            m_grabOffset = glm::vec2(mouseX, mouseY) - balls[i].position;
            m_prevPos = balls[i].position;
            balls[i].velocity = glm::vec2(0.0f);
        }
    }
}

void BallController::onMouseUp(std::vector <Ball>& balls) {
    if (m_grabbedBall != -1) {
        // Throw the ball!
        balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
        m_grabbedBall = -1;
    }
}

void BallController::onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY) {
    if (m_grabbedBall != -1) {
        balls[m_grabbedBall].position = glm::vec2(mouseX, mouseY) - m_grabOffset;
    }
}

void BallController::updateCollision(Grid* grid) {
    for (size_t i = 0; i < grid->m_cells.size(); i++) {

        int x = i % grid->m_numXCells;
        int y = i / grid->m_numXCells;

        Cell& cell = grid->m_cells[i];

        // Loop through all balls in a cell
        for (size_t j = 0; j < cell.balls.size(); j++) {
            Ball* ball = cell.balls[j];
            /// Update with the residing cell
            checkCollision(ball, cell.balls, j + 1);

            /// Update collision with neighbor cells
            if (x > 0) {
                // Left
                checkCollision(ball, grid->getCell(x - 1, y)->balls, 0);
                if (y > 0) {
                    /// Top left
                    checkCollision(ball, grid->getCell(x - 1, y - 1)->balls, 0);
                }
                if (y < grid->m_numYCells - 1) {
                    // Bottom left
                    checkCollision(ball, grid->getCell(x - 1, y + 1)->balls, 0);
                }
            }
            // Up cell
            if (y > 0) {
                checkCollision(ball, grid->getCell(x, y - 1)->balls, 0);
            }
        }
    }
}

void BallController::checkCollision(Ball* ball, std::vector<Ball*>& ballsToCheck, int startingIndex) {
    for (size_t i = startingIndex; i < ballsToCheck.size(); i++) {
        checkCollision(*ball, *ballsToCheck[i]);
    }
}

void BallController::checkCollision(Ball& b1, Ball& b2) {
    // We add radius since position is the top left corner
    glm::vec2 distVec = b2.position - b1.position;
    glm::vec2 distDir = glm::normalize(distVec);
    float dist = glm::length(distVec);
    float totalRadius = b1.radius + b2.radius;

    float collisionDepth = totalRadius - dist;
    // Check for collision
    if (collisionDepth > 0) {

        // Push away the balls based on ratio of masses
        b1.position -= distDir * collisionDepth * (b2.mass / b1.mass) * 0.5f;
        b2.position += distDir * collisionDepth * (b1.mass / b2.mass) * 0.5f;

        // Calculate deflection. http://stackoverflow.com/a/345863
        // Fixed thanks to youtube user Sketchy502
        float aci = glm::dot(b1.velocity, distDir);
        float bci = glm::dot(b2.velocity, distDir);

        float acf = (aci * (b1.mass - b2.mass) + 2 * b2.mass * bci) / (b1.mass + b2.mass);
        float bcf = (bci * (b2.mass - b1.mass) + 2 * b1.mass * aci) / (b1.mass + b2.mass);

        b1.velocity += (acf - aci) * distDir;
        b2.velocity += (bcf - bci) * distDir;

        if (glm::length(b1.velocity + b2.velocity) > 0.5f) {
            // Choose the faster ball
            bool choice = glm::length(b1.velocity) < glm::length(b2.velocity);

            // Faster ball transfers it's color to the slower ball
            choice ? b2.color = b1.color : b1.color = b2.color;
        }
    }
}


bool BallController::isMouseOnBall(Ball&b, float mouseX, float mouseY) {
    return (mouseX >= b.position.x - b.radius && mouseX < b.position.x + b.radius &&
            mouseY >= b.position.y - b.radius && mouseY < b.position.y + b.radius);
}

glm::vec2 BallController::getGravityAccel() {
    const float GRAVITY_FORCE = 0.02f;
    glm::vec2 gravity;

    switch (m_gravityDirection) {
        case GravityDirection::DOWN:
            gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
            break;
        case GravityDirection::LEFT:
            gravity = glm::vec2(-GRAVITY_FORCE, 0.0f);
            break;
        case GravityDirection::RIGHT:
            gravity = glm::vec2(GRAVITY_FORCE, 0.0f);
            break;
        case GravityDirection::UP:
            gravity = glm::vec2(0.0f, GRAVITY_FORCE);
            break;
        default:
            gravity = glm::vec2(0.0f);
            break;
    }
    return gravity;
}
