#include "prioritysheet.h"
#include "../game.h"

using namespace agi;

PicBatch::PicBatch(int camera, const std::string &picName) :
	Batch<VertexPalette>(4, 6, GL_TRIANGLES, 1, 5, camera) {
	_indexPattern = {0, 1, 2, 0, 2, 3};
	auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";

	_pic = std::make_unique<IndexedPNG>(assetPath + picName + "v.png");
	_priority = std::make_unique<IndexedPNG>(assetPath + picName + "p.png");
	_pic->load();

	std::vector<float> grayscalePixels(_priority->size());
	for (size_t i = 0; i < _priority->size(); ++i) {

		unsigned char v = (*_priority)[i];
		// Convert index to grayscale using your formula
		float u = 1.0f - (static_cast<float>(v) - 3.0f) / 13.0f;
		grayscalePixels[i] = u;
	}
	glGenTextures (1, &_priorityTex);
	glBindTexture(GL_TEXTURE_2D, _priorityTex);
	glTexImage2D(
		GL_TEXTURE_2D, 0,
		GL_R32F, // One-channel float texture
		_priority->getWidth(), _priority->getHeight(),
		0,
		GL_RED, GL_FLOAT,
		grayscalePixels.data()
	);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// don't load the priority! Transform first into grayscale image!
	//_priority->load();
	_texWidth = _pic->getWidth();
	_texHeight = _pic->getHeight();
	_invWidth = 1.f / _texWidth;
	_invHeight = 1.f / _texHeight;
}

void PicBatch::setupUniforms(Shader * s) {
	IBatch::setupUniforms(s);
	// Activate texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _pic->getTexId());
	GLint locDiffuse = glGetUniformLocation(s->getProgId(), "texture_pdiffuse1");
	glUniform1i(locDiffuse, 0);

	// Activate and bind texture_palette to texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _pic->getPalId());
	GLint locPalette = glGetUniformLocation(s->getProgId(), "texture_palette");
	glUniform1i(locPalette, 1); // Bind to texture unit 1

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _priorityTex);
	GLint locPriority = glGetUniformLocation(s->getProgId(), "uPriorityTex");
	glUniform1i(locPriority, 2);

}


