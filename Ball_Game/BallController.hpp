#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Grid.hpp"
#include "Vertex.hpp"
#include "SpriteBatch.hpp"
#include "GLSLProgram.hpp"

enum class GravityDirection {NONE, LEFT, UP, RIGHT, DOWN};

class Cell;
class Grid;

// Ball.h
struct Ball {
    Ball(float radius, float mass, const glm::vec2& pos,
         const glm::vec2& vel, unsigned int textureId,
         const ColorRGBA8& color);

    float radius;
    float mass;
    glm::vec2 velocity;
    glm::vec2 position;
    unsigned int textureId = 0;
    ColorRGBA8 color;
    Cell* ownerCell = nullptr;
    int cellVectorIndex = -1;
};

// BallRenderer.h
class BallRenderer {
public:
    virtual void renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix);
protected:
    std::unique_ptr<GLSLProgram> m_program = nullptr;
};

// Visualizes kinetic energy
class MomentumBallRenderer : public BallRenderer {
public:
    virtual void renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
};

// Visualizes positive X component of velocity, as well as position
class VelocityBallRenderer : public BallRenderer {
public:
    VelocityBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
};

// Trippy renderer!
class TrippyBallRenderer : public BallRenderer {
public:
    TrippyBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
    float m_time = 0.0f;
};

// Ball Controller

class BallController {
public:
    /// Updates the balls
    void updateBalls(std::vector <Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY);
    /// Some simple event functions
    void onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY);
    void onMouseUp(std::vector <Ball>& balls);
    void onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY);
    void setGravityDirection(GravityDirection dir) { m_gravityDirection = dir; }
private:
    // Updates collision
    void updateCollision(Grid* grid);
    /// Checks collision between a ball and a vector of balls, starting at a specific index
    void checkCollision(Ball* ball, std::vector<Ball*>& ballsToCheck, int startingIndex);
    /// Checks collision between two balls
    void checkCollision(Ball& b1, Ball& b2);
    /// Returns true if the mouse is hovering over a ball
    bool isMouseOnBall(Ball&b, float mouseX, float mouseY);
    glm::vec2 getGravityAccel();

    int m_grabbedBall = -1; ///< The ball we are currently grabbing on to
    glm::vec2 m_prevPos = glm::vec2(0.0f); ///< Previous position of the grabbed ball
    glm::vec2 m_grabOffset = glm::vec2(0.0f); ///< Offset of the cursor on the selected ball

    GravityDirection m_gravityDirection = GravityDirection::NONE;
};
