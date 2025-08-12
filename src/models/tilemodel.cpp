#include "tilemodel.h"
#include "../linebatch.h"

using namespace models;

void Animation::addFrame(const Frame& frame) {

	_frames.push_back(frame);
}


void TileMap::setOffset(glm::vec2 offset) {
	_offset = offset;
	if (_autoDraw) {
		draw();

	}
}
TileMap::TileMap(int batchId, int n) : Model<VertexPalette>(batchId), _currentFrame(0), _currentTick(0),
	_offset(0.f), _quadCount(n), _autoDraw(true), _firstCallToSet(true) {
	allocate(n);
	auto batch = dynamic_cast<QuadBatchPalette*>(_batch);
	if (batch == nullptr) {
		GLIB_FAIL("Text objects require a Quad Batch Palette!");
	}
	_texSize = batch->getSpriteSheet()->getTextureSize();

}

int TileMap::addQuad(const QuadInfo& info) {

	_quads.push_back(info);
	return _quads.size() - 1;
}
void TileMap::addQuad(int x, int y, int width, int height, int pal, glm::vec2 pos, glm::vec2 scale) {
//
//	QuadInfo info;
//	info.tx = (float)x / _texSize[0];
//	info.ty = (float)y / _texSize[1];
//	info.tw = (float)width / _texSize[0];
//	info.th = (float)height / _texSize[1];
//	info.sizex = width * scale.x;
//	info.sizey = height * scale.y;
//	info.x = pos.x;
//	info.y = pos.y;
//	info.pal = pal;
//	_quads.push_back(info);
}

void TileMap::addAnimation(const std::string& id, const Animation& anim) {
	if (_animations.empty()) _currentAnimation = id;
	_animations[id] = anim;
}

glm::vec2 TileMap::getBonePosition(int boneId) const {
	auto& anim = _animations.at(_currentAnimation);
	auto& frame = anim.getFrame(_currentFrame);
	return _quads[frame.quads[0].id].bones[boneId];
}

void TileMap::setEndOfLoop(pybind11::function f) {
	_endOfLoop = f;
}
void TileMap::updateImpl() {
	auto& anim = _animations.at(_currentAnimation);
	auto& frame = anim.getFrame(_currentFrame);
	bool refreshModel = _node->hasMoved();
	bool eol{false};
	if (frame.ticks != 0) {
		// if ticks is 0, then no animation!
		_currentTick++;
		if (_currentTick > frame.ticks) {
			// advance frame!
			refreshModel = true;
			_currentTick = 0;
			_currentFrame++;
			if (_currentFrame >= anim.getFrameCount()) {
				_currentFrame = anim.getLoopFrame();
				eol = true;
			}
			auto& newFrame = anim.getFrame(_currentFrame);
			for (int i = 0; i < _quadCount; ++i) {
				if (frame.quads[i].id == -1 && newFrame.quads[i].id >= 0) {
					// show
				} else if (frame.quads[i].id >= 0 && newFrame.quads[i].id == -1) {
					// hide)
				}
			}
		}
	}

	if (refreshModel) {
		draw();
	}
	// if end of loop --> callback
	if (eol && _endOfLoop) {
		_endOfLoop();
	}

}


void TileMap::setAnimation(const std::string & id) {
	if (_currentAnimation != id || _firstCallToSet) {
		M_Assert(_animations.count(id) > 0, "Animation: " + id + " does not exist!");
		_currentAnimation = id;
		_currentTick = 0;
		_currentFrame = 0;
		_firstCallToSet = false;
		draw();
	}
}

void TileMap::setPalette(int id) {
	for (auto& q : _quads) q.pal = id;
	if (_node != nullptr) {
		draw();
	}
}
void TileMap::draw() {
	// TODO allow for flipx and scale
	// if flipped, bottomLeft is bottom right
	auto flipx = _node->getFlipHorizontal();
	auto bottomLeft = _node->getWorldPosition().toGlm();
	//std::cout << bottomLeft[0] << "\n";
	auto& frame = _animations.at(_currentAnimation).getFrame(_currentFrame);
	int i = 0;
	//if (_currentFrame >= _frames.size()) {
	//	GLIB_FAIL("[TILEMAP] Error! Frame " << _currentFrame << " does not exist!");
	//}
	for (const auto& quad : frame.quads) {
		if (quad.id != -1) {
			float z = bottomLeft.z + quad.layer * 0.01f;
			bool fx = (flipx != quad.flipx);
			float ffx = fx ? -1.f : 1.f;
			//bool fy = quad.flipy;
			const auto& q = _quads[quad.id];
			auto quadBottomLeft = bottomLeft +
					glm::vec3(ffx*_offset.x, _offset.y, 0.f) +
					glm::vec3(ffx*q.x, q.y, 0.f) +
					glm::vec3( ffx*quad.position.x, quad.position.y, 0.f);
			quadBottomLeft.z = z;
			glm::vec2 size (q.sizex * quad.scale.x, q.sizey * quad.scale.y);
			// TOP LEFT
			_vertices[i][0].texCoord = {q.tx0, q.ty0};
			_vertices[i][0].paletteId = q.pal;
			_vertices[i][0].position = quadBottomLeft + glm::vec3(0.f, size.y, 0.f);

			// Bottom LEFT
			_vertices[i][1].texCoord = {q.tx0, q.ty1};
			_vertices[i][1].paletteId = q.pal;
			_vertices[i][1].position = quadBottomLeft;

			// Bottom RIGHT
			_vertices[i][2].texCoord = {q.tx1, q.ty1};
			_vertices[i][2].paletteId = q.pal;
			_vertices[i][2].position = quadBottomLeft + glm::vec3(ffx*size.x, 0.f, 0.f);

			// Top RIGHT
			_vertices[i][3].texCoord = {q.tx1, q.ty0};
			_vertices[i][3].paletteId = q.pal;
			_vertices[i][3].position = quadBottomLeft + glm::vec3(ffx*size.x, size.y, 0.f);

//			if (fx) {
//				_vertices[i][0].position.x *= -1.f;
//				_vertices[i][1].position.x *= -1.f;
//				_vertices[i][2].position.x *= -1.f;
//				_vertices[i][3].position.x *= -1.f;
//
//				//std::swap(_vertices[i][0].position.x, _vertices[i][2].position.x);
//				//std::swap(_vertices[i][1].position.x, _vertices[i][3].position.x);
//			}
		}
		i++;
	}
}

bool TileMap::hasAnimation(const std::string &anim) const {
	return _animations.count(anim) != 0;
}