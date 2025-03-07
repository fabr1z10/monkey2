#include "quadbatch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "error.h"
#include "game.h"


void QuadBatch::configure() {
    Batch<primitives::Quad>::configure();


}

int QuadBatch::addTexture(const std::string &filePath) {




    auto it = _texId.find(filePath);
    if (it != _texId.end()) {
        return it->second;
    }

    int width, height, nrChannels;
    auto assetFile = Game::instance().getWorkingDirectory() + "/assets/" + filePath;
    unsigned char* data = stbi_load(assetFile.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        GLIB_FAIL("Failed to load texture " + assetFile);
    }
    if (_texCount == 0) {
        _texWidth = width;
        _texHeight = height;
        // if this is the 1st texture, generate gl stuff
        glGenTextures(1, &_textureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _texWidth, _texHeight, _maxTextures);

        // Set texture parameters
        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }


    std::cout << " -- loaded " << filePath << " (" << width << ", " << height << ", " << nrChannels << ")\n";
    // Make sure the texture fits the array size (image should match the array dimensions)
    if (width != _texWidth || height != _texHeight) {
        stbi_image_free(data);
        GLIB_FAIL( "Texture size does not match the texture array size!");
    }

    // Upload texture to the texture array
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, _texCount, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    _texId[filePath] = _texCount;
    // Increment the texture count
    return _texCount++;
}

void QuadBatch::setupUniforms(Shader *s) {
    IBatch::setupUniforms(s);
    // Activate texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture array to texture unit 0
    glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
    GLint boundTex = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &boundTex);
    //std::cout << "Bound texture array ID: " << boundTex << " (should be " << _textureArray << ")" << std::endl;
    // Set the uniform to use texture unit 0 (where we bound the texture array)
    GLuint texLocation = glGetUniformLocation(s->getProgId(), "textureArray");
    glUniform1i(texLocation, 0);

}
