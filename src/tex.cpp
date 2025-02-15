#include "tex.h"
#include "stb_image.h"
#include "error.h"
#include <iostream>



Tex::Tex(const std::string &filePath) {
    unsigned char* data = stbi_load(filePath.c_str(), &_width, &_height, &_nrChannels, 0);
    if (!data) {
        GLIB_FAIL("Failed to load texture " + filePath);
    }
    std::cout << " -- loaded " << filePath << " (" << _width << ", " << _height << ", " << _nrChannels << ")\n";

    glGenTextures (1, &_texId);
    glBindTexture (GL_TEXTURE_2D, _texId);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8 , _width, _height, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
