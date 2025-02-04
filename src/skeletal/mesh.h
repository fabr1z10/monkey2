#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

class Shader;

class Mesh {
public:
    Mesh(const std::vector<float>& points, const std::vector<unsigned>& indices, const std::string& texture);
    void draw(Shader*);
private:
    void generateBuffers(const std::vector<float>& points, const std::vector<unsigned>& indices);
    GLuint _size;
    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;
    GLuint _texId;
};