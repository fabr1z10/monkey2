#include "mesh.h"
#include "../shader.h"
#include "../assetmanager.h"
#include "../primitives/vertex.h"

Mesh::Mesh(const std::vector<float> &points, const std::vector<unsigned int> &indices, const std::string& texture) {
    _texId = AssetManager::instance().getTextureId(texture);
    generateBuffers(points, indices);
}

void Mesh::generateBuffers(const std::vector<float>& vertices, const std::vector<unsigned>& indices) {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Define position attribute (3 floats per vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSkeletal), (void*)0);
    glEnableVertexAttribArray(0);

    // Define texture coordinate attribute (2 floats per vertex)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexSkeletal), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Define weights attribute (3 floats per vertex)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSkeletal), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);
    _size = indices.size();
}

void Mesh::draw(Shader* s) {
    if (_texId != GL_INVALID_VALUE) {
        s->setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texId);
    }
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));
    glBindVertexArray(0);
}
