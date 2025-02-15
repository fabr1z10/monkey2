#pragma once

#include <string>
#include <GL/glew.h>

class Tex {

public:
    Tex(const std::string& file);
    int getWidth() const;
    int getHeight() const;
private:
    GLuint _texId;
    int _width;
    int _height;
    int _nrChannels;
    //static std::unordered_map<std::string, GLuint> _textures;
};

inline int Tex::getWidth() const {
    return _width;
}

inline int Tex::getHeight() const {
    return _height;
}
