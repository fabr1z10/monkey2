#include "tex.h"
#include "stb_image.h"
#include "error.h"
#include <iostream>

std::unordered_map<std::string, GLuint> Tex::_textures;

GLuint Tex::getTexture(const std::string &filePath) {
    auto it = Tex::_textures.find(filePath);
    if (it == _textures.end()) {

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            GLIB_FAIL("Failed to load texture " + filePath);
        }
        std::cout << " -- loaded " << filePath << " (" << width << ", " << height << ", " << nrChannels << ")\n";
        GLuint texId;
        glGenTextures (1, &texId);
        glBindTexture (GL_TEXTURE_2D, texId);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8 , width, height, 0,  GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        _textures[filePath] = texId;
        return texId;
    } else {
        return it->second;
    }
}