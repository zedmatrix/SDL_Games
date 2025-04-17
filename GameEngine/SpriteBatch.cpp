#include "SpriteBatch.hpp"

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
    texture(Texture),
    depth(Depth) {

    topLeft.color = color;
    topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = color;
    bottomLeft.setPosition(destRect.x, destRect.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = color;
    bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = color;
    topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
             GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
    texture(Texture),
    depth(Depth) {

    // Get Points Centered at origin
    glm::vec2 halfDim(destRect.z / 2.0f, destRect.w / 2.0f);

    glm::vec2 tL(-halfDim.x, halfDim.y);
    glm::vec2 bL(-halfDim.x, -halfDim.y);
    glm::vec2 bR(halfDim.x, -halfDim.y);
    glm::vec2 tR(halfDim.x, halfDim.y);

    // Rotate the points
    tL = rotatePoint(tL, angle) + halfDim;
    bL = rotatePoint(bL, angle) + halfDim;
    bR = rotatePoint(bR, angle) + halfDim;
    tR = rotatePoint(tR, angle) + halfDim;

    topLeft.color = color;
    topLeft.setPosition(destRect.x + tL.x, destRect.y + tL.y);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = color;
    bottomLeft.setPosition(destRect.x + bL.x, destRect.y + bL.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = color;
    bottomRight.setPosition(destRect.x + bR.x, destRect.y + bR.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = color;
    topRight.setPosition(destRect.x + tR.x, destRect.y + tR.y);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    //std::cout << "UVrect: " << uvRect.z << "," << uvRect.w << std::endl;

}
glm::vec2 Glyph::rotatePoint(const glm::vec2& pos, float angle) {
    glm::vec2 newV;
    newV.x = pos.x * cos(angle) - pos.y * sin(angle);
    newV.y = pos.x * sin(angle) + pos.y * cos(angle);
    return newV;
}

/// SpriteBatch
SpriteBatch::~SpriteBatch() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
}
void SpriteBatch::init() {
    createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /*GlyphSortType::TEXTURE */) {
    m_sortType = sortType;
    m_renderBatches.clear();
    m_glyphs.clear();
}

void SpriteBatch::end() {
    sortGlyphs();
    createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect,
                       const glm::vec4& uvRect,
                       GLuint texture, float depth,
                       const ColorRGBA8& color) {

    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}
// overload
void SpriteBatch::draw(const glm::vec4& destRect,
                       const glm::vec4& uvRect,
                       GLuint texture, float depth,
                       const ColorRGBA8& color, float angle) {
    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::createRenderBatches() {
    std::vector <Vertex> vertices;
    vertices.resize(m_glyphs.size() * 6);

    if (m_glyphs.empty()) return;

    int offset{0};
    int currentVertex{0};
    m_renderBatches.emplace_back(offset, 6, m_glyphs[0].texture);
    vertices[currentVertex++] = m_glyphs[0].topLeft;
    vertices[currentVertex++] = m_glyphs[0].bottomLeft;
    vertices[currentVertex++] = m_glyphs[0].bottomRight;
    vertices[currentVertex++] = m_glyphs[0].bottomRight;
    vertices[currentVertex++] = m_glyphs[0].topRight;
    vertices[currentVertex++] = m_glyphs[0].topLeft;
    offset += 6;

    for (size_t currentGlyph = 1; currentGlyph < m_glyphs.size(); currentGlyph++) {

        if (m_glyphs[currentGlyph].texture != m_glyphs[currentGlyph - 1].texture) {
            m_renderBatches.emplace_back(offset, 6, m_glyphs[currentGlyph].texture);
        } else {
            m_renderBatches.back().numVertices += 6;
        }
        vertices[currentVertex++] = m_glyphs[currentGlyph].topLeft;
        vertices[currentVertex++] = m_glyphs[currentGlyph].bottomLeft;
        vertices[currentVertex++] = m_glyphs[currentGlyph].bottomRight;
        vertices[currentVertex++] = m_glyphs[currentGlyph].bottomRight;
        vertices[currentVertex++] = m_glyphs[currentGlyph].topRight;
        vertices[currentVertex++] = m_glyphs[currentGlyph].topLeft;
        offset += 6;

    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
void SpriteBatch::createVertexArray() {

    if (m_vao == 0) glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    if (m_vbo == 0) glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Position Attribute pointer // (void*)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);

}
void SpriteBatch::sortGlyphs() {
    switch (m_sortType) {
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(m_glyphs.begin(), m_glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.depth < b.depth; });
            break;
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(m_glyphs.begin(), m_glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.depth > b.depth; });
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(m_glyphs.begin(), m_glyphs.end(),
                [](const Glyph& a, const Glyph& b) { return a.texture < b.texture; });
            break;
        case GlyphSortType::NONE:
            break;
    }
}

void SpriteBatch::renderBatch() {
    glBindVertexArray(m_vao);
    GLuint lastTexture = 0;
    for (const auto& batch : m_renderBatches) {
        if (batch.texture != lastTexture) {
            glBindTexture(GL_TEXTURE_2D, batch.texture);
            lastTexture = batch.texture;
        }
        glDrawArrays(GL_TRIANGLES, batch.offset, batch.numVertices);
    }
    glBindVertexArray(0);
}
