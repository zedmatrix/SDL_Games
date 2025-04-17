#include "IOManager.hpp"
#include "picopng.hpp"
#include "Error.hpp"
#include <fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.fail()) {
        std::perror(filePath.c_str());
        return false;
    }
    //seek to end of file
    file.seekg(0, std::ios::end);

    // get file size
    int fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reduce file size by any header bytes
    fileSize -= file.tellg();

    buffer.resize(fileSize);
    file.read((char*)&(buffer[0]), fileSize);

    file.close();
    return true;

}

//ImageLoader
GLTexture IOManager::loadPNG(std::string filePath) {
    GLTexture texture{};
    std::vector<unsigned char> in;
    std::vector<unsigned char> out;

    unsigned long width, height;
    if (readFileToBuffer(filePath, in) == false) {
        fatalError("Failed to Load PNG File to buffer");
    }


    int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
    if (errorCode != 0) {
        fatalError("decodePNG failed with error code " + std::to_string(errorCode));
    }
    glGenTextures(1, &(texture.id));

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture.width = width;
    texture.height = height;

    return texture;

}

