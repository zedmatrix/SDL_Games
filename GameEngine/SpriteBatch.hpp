#ifndef SPRITEBATCH_HPP
#define SPRITEBATCH_HPP
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <random>
#include <string>
#include "GLTexture.hpp"
#include "Vertex.hpp"

enum class GlyphSortType {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

struct RenderBatch {
    GLuint offset;
    GLuint numVertices;
    GLuint texture;

    RenderBatch(GLuint off, GLuint numVerts, GLuint tex)
        : offset(off), numVertices(numVerts), texture(tex) {}
};

class Glyph {
public:
    Glyph() {};
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture,
          float Depth, const ColorRGBA8& color);
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture,
          float Depth, const ColorRGBA8& color, float angle);

    GLuint texture;
    float depth;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;
private:
    // Rotates a point about (0,0) by angle
    glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
};

class SpriteBatch {

public:
    SpriteBatch() : m_vbo(0), m_vao(0) {}
    ~SpriteBatch();
    void init();
    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture,
              float depth, const ColorRGBA8& color);
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture,
              float depth, const ColorRGBA8& color, float angle);
    void renderBatch();

private:
    void createRenderBatches();
    void createVertexArray();
    void sortGlyphs();
    GLuint m_vbo;
    GLuint m_vao;
    std::vector<Glyph> m_glyphs;
    std::vector<RenderBatch> m_renderBatches;
    GlyphSortType m_sortType;

};

#endif //SPRITEBATCH_HPP
