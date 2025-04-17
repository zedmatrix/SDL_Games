#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#include "TextureCache.hpp"
#include "GLTexture.hpp"

class ResourceManager {

public:
    static GLTexture getTexture(std::string texturePath);
    //static GLuint boundTexture()
private:

    static TextureCache _textureCache;
};


#endif //RESOURCEMANAGER_HPP
