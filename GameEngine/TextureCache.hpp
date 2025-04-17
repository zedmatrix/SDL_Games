#ifndef TEXTURECACHE_HPP
#define TEXTURECACHE_HPP
#include <map>
#include <string>
#include "GLTexture.hpp"

class TextureCache {

public:
    TextureCache();
    ~TextureCache();

    GLTexture getTexture(std::string texturePath);

private:
    std::map<std::string, GLTexture> _textureMap;
};



#endif //TEXTURECACHE_HPP
