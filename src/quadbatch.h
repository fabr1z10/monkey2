#pragma once

#include "batch.h"
#include "primitives/prim.h"

class QuadBatch : public Batch<primitives::Quad> {
public:
    QuadBatch(int size, int cam, int maxTextures) : Batch<primitives::Quad>(size, cam),
        _maxTextures(maxTextures), _texCount(0) {

    }

    void configure() override ;

    void setupUniforms(Shader* s) override;

    int addTexture(const std::string& filename);

    float getTextureWidth() const;

    float getTextureHeight() const;

    glm::vec2 getTextureSize() const;


private:
    GLuint _textureArray;
    float _texWidth;
    float _texHeight;
    int _maxTextures;
    int _texCount;
    std::unordered_map<std::string, int> _texId;
};

inline float QuadBatch::getTextureWidth() const {
    return _texWidth;
}

inline float QuadBatch::getTextureHeight() const {
    return _texHeight;
}

inline glm::vec2 QuadBatch::getTextureSize() const {
    return {_texWidth, _texHeight};
}
