#include "quad.h"

using namespace models;

Quad::Quad(int batchId, int x, int y, int width, int height, int pal) :
	Model<VertexPalette>(batchId), _quad(x, y,width,height), _pal(pal) {
	allocate(1);
}

void Quad::update() {
	draw();
}

void Quad::draw() {
	auto texCoords = _batch->pixelToTextureCoords(_quad);
	auto bottomLeft = _node->getWorldPosition().toGlm();

	_vertices[0]->position = bottomLeft + glm::vec3(0.f, _quad[3], 0.f);
	_vertices[0]->texCoord = glm::vec2(texCoords[0], texCoords[1]);
	_vertices[0]->paletteId = _pal;

	_vertices[0][1].position = bottomLeft;
	_vertices[0][1].texCoord = glm::vec2(texCoords[0], texCoords[1] + texCoords[3]);
	_vertices[0][1].paletteId = _pal;

	_vertices[0][2].position = bottomLeft + glm::vec3(_quad[2], 0.f, 0.f);
	_vertices[0][2].texCoord = glm::vec2(texCoords[0] + texCoords[2], texCoords[1] + texCoords[3]);
	_vertices[0][2].paletteId = _pal;

	_vertices[0][3].position = bottomLeft + glm::vec3(_quad[2], _quad[3], 0.f);
	_vertices[0][3].texCoord = glm::vec2(texCoords[0] + texCoords[2], texCoords[1]);
	_vertices[0][3].paletteId = _pal;

}
