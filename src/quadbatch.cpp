#include "quadbatch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "error.h"


void QuadBatch::configure() {
    Batch<primitives::Quad>::configure();


}

void QuadBatch::addTexture(const std::string &filePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        GLIB_FAIL("Failed to load texture " + filePath);
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

    // Increment the texture count
    ++_texCount;
}

void QuadBatch::setupUniforms(Shader *s) {
    IBatch::setupUniforms(s);
    // Activate texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture array to texture unit 0
    glBindTexture(GL_TEXTURE_2D_ARRAY, _textureArray);
    GLint boundTex = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &boundTex);
    std::cout << "Bound texture array ID: " << boundTex << " (should be " << _textureArray << ")" << std::endl;
    // Set the uniform to use texture unit 0 (where we bound the texture array)
    GLuint texLocation = glGetUniformLocation(s->getProgId(), "textureArray");
    glUniform1i(texLocation, 0);

}