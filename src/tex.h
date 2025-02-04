#pragma once

#include <string>
#include <GL/glew.h>
#include <unordered_map>

class Tex {

public:

    static GLuint getTexture(const std::string& file);



private:
    static std::unordered_map<std::string, GLuint> _textures;
};