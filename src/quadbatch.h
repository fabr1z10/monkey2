/*
#pragma once

#include "batch.h"
#include "primitives/prim.h"
#include <yaml-cpp/yaml.h>

class PaletteQuadBatch : public Batch<primitives::QuadPalette> {
public:
	PaletteQuadBatch(int size, int cam, const std::string& texture, const YAML::Node& palettes);

	void setupUniforms(Shader* s) override;

private:
	uint32_t packRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void unpackRGBA(uint32_t color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
	GLuint _texId;
	GLuint _palId;
};




class QuadBatch : public Batch<primitives::Quad> {
public:
    QuadBatch(int size, int cam, int maxTextures) : Batch<primitives::Quad>(size, cam),
        _maxTextures(maxTextures), _texCount(0) {

    }

    void configure() override ;

    void setupUniforms(Shader* s) override;

    int addTexture(const std::string& filename);


    glm::ivec2 getTextureSize() const;


private:
    GLuint _textureArray;
    int _maxTextures;
    int _texCount;
    std::unordered_map<std::string, int> _texId;
};


inline glm::ivec2 QuadBatch::getTextureSize() const {
    return {_texWidth, _texHeight};
}
*/
