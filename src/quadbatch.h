#include "batch.h"
#include "primitives/prim.h"

class QuadBatch : public Batch<primitives::Quad> {
public:
    QuadBatch(int size, int cam, float width, float height, int maxTextures) : Batch<primitives::Quad>(size, cam),
            _texWidth(width), _texHeight(height), _maxTextures(maxTextures), _texCount(0) {
        glGenTextures(1, &_textureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _texWidth, _texHeight, _maxTextures);

        // Set texture parameters
        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void configure() override ;

    void setupUniforms(Shader* s) override;

    void addTexture(const std::string& filename);

    float getTextureWidth() const;

    float getTextureHeight() const;
private:
    GLuint _textureArray;
    float _texWidth;
    float _texHeight;
    int _maxTextures;
    int _texCount;
};

inline float QuadBatch::getTextureWidth() const {
    return _texWidth;
}

inline float QuadBatch::getTextureHeight() const {
    return _texHeight;
}