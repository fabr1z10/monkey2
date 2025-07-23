#pragma once


#include "batch.h"
#include "primitives/vertex.h"
#include "spritesheet.h"


class LineBatch : public Batch<VertexColor> {
public:
	LineBatch(int lines, int camera) : Batch<VertexColor>(2, 2, GL_LINES,lines, 1, camera) {
		_indexPattern = {0, 1};
	}
};

class QuadBatch : public Batch<VertexTexture> {
public:
	QuadBatch(int quads, int camera) : Batch<VertexTexture>(4, 6, GL_TRIANGLES, quads, 3, camera) {
		_indexPattern = {0, 1, 2, 0, 2, 3};

	}
	//void setupUniforms(Shader* s);
};


class QuadBatchPalette : public Batch<VertexPalette> {
public:
	QuadBatchPalette(int quads, int camera, const std::string& spritesheet);

	void setupUniforms(Shader*);

	SpriteSheet* getSpriteSheet();
private:
	std::shared_ptr<SpriteSheet> _sheet;
};

inline SpriteSheet *QuadBatchPalette::getSpriteSheet() {
	return _sheet.get();
}