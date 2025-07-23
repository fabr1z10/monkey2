#include "linebatch.h"
#include "game.h"
#include "error.h"






QuadBatchPalette::QuadBatchPalette(int quads, int camera, const std::string& spritesheet) :
		Batch<VertexPalette>(4, 6, GL_TRIANGLES, quads, 4, camera) {

	_indexPattern = {0, 1, 2, 0, 2, 3};

	_sheet = SpriteSheet::getSheet(spritesheet);
	auto texSize = _sheet->getTextureSize();
	_texWidth = texSize[0];
	_texHeight = texSize[1];
	_invWidth = 1.f / _texWidth;
	_invHeight = 1.f / _texHeight;
}



void QuadBatchPalette::setupUniforms(Shader * s) {
	IBatch::setupUniforms(s);
	_sheet->setupUniforms(s);

}

