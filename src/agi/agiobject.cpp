#include "agiobject.h"
#include "agiroom.h"
#include "../game.h"
#include "../linebatch.h"

extern GLFWwindow* window;

using namespace agi;

const std::unordered_map<int, int> AGIObject::_keyDir = {
		{0x00, 0}, {0x01, 3}, {0x02, 7}, {0x03, 0}, {0x04, 5}, {0x05, 4}, {0x06, 6}, {0x07, 5},
		{0x08, 1}, {0x09, 2}, {0x0A, 8}, {0x0B, 1}, {0x0C, 0}, {0x0D, 3}, {0x0E, 7}, {0x0F, 0}
};


void AGIObject::setEndOfLoop(pybind11::function f) {
	auto * tm = dynamic_cast<models::TileMap*>(_model.get());
	tm->setEndOfLoop(f);
}
void AGIObject::setView(const std::string & viewId) {
	auto* batch = dynamic_cast<QuadBatchPalette*>(_room->getBatch(1));
	std::string currentAnimation;
	if (_model) {
		currentAnimation = _model->getAnimation();
	}

	auto model = batch->getSpriteSheet()->makeSprite(1, viewId);
	this->setModel(model);

	if (!currentAnimation.empty() && model->hasAnimation(currentAnimation)) {
		model->setAnimation(currentAnimation);
		//model->updateImpl();
	}
	model->updateImpl();


}

AGIObject::AGIObject(const std::string &viewId, int x, int y, float speed) : Node(), _x(x), _y(y), _horizontalMove(0), _verticalMove(0),
_refresh(5), _ticks(0), _accumulatedTime(0.), _speed(speed), _dir(0), _motion(true) {

	_room = dynamic_cast<agi::AGIRoom*>(Game::instance().getRoom());
	auto* batch = dynamic_cast<QuadBatchPalette*>(_room->getBatch(1));
	auto model = batch->getSpriteSheet()->makeSprite(1, viewId);
	this->setModel(model);

	_hasUpDownAnim = model->hasAnimation("2") && model->hasAnimation("3");
	_hasLeftAnim = model->hasAnimation("1");

	computePriority();
	//this->setPosition(Vec3(_x, _y, 0.f));
	// the last 4 bits are UP, DOWN, LEFT, RIGHT
	// Direction 	Loop number
	// 0 (not moving) 	Not changed
//	1 (up) 	3 (if loops 2 and 3 exist; otherwise not changed)
//	2 (up-right) 	0
//	3 (right) 	0
//	4 (down-right) 	0
//	5 (down) 	2 (if loops 2 and 3 exist; otherwise not changed)
//	6 (down-left) 	1 (if loop 1 exists; otherwise loop 0)
//	7 (left) 	1 (if loop 1 exists; otherwise loop 0)
//	8 (up-left) 	1 (if loop 1 exists; otherwise loop 0)

}

void AGIObject::computePriority() {
	int yt = 167 - static_cast<int>(_y);
	if (yt < 48) {
		_priority = 4;
	} else {
		_priority = 1 + (yt / 12);
	}
	float z = -(1.f - (_priority - 3.f)/13.f) + 0.039f;
	std::cout << "y = " << yt << ", prio = " << _priority << "\n";
	this->setPosition(Vec3(_x, _y, z));

}


void AGIObject::customUpdate(double dt) {

	if (!_motion) return;
	auto upDown = glfwGetKey(window, GLFW_KEY_UP);
	auto downDown = glfwGetKey(window, GLFW_KEY_DOWN);
	auto leftDown = glfwGetKey(window, GLFW_KEY_LEFT);
	auto rightDown = glfwGetKey(window, GLFW_KEY_RIGHT);
	int keyMask =
			(upDown ? 0x08 : 0x00) |
			(downDown ? 0x04 : 0x00) |
			(leftDown ? 0x02 : 0x00) |
			(rightDown ? 0x01 : 0x00);
	_dir = _keyDir.at(keyMask);


	_horizontalMove = (leftDown ^ rightDown) ? (leftDown ? -1.f : 1.f) : 0.f;
	_verticalMove = (upDown ^ downDown) ? (upDown ? 1.f : -1.f) : 0.f;


//	_verticalMove = 0.f;
//	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//		_verticalMove = 1.f;
//	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//		_verticalMove = -1.f;
//	}
//	_horizontalMove = 0.f;
//	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//		_horizontalMove = 1.f;
//	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//		_horizontalMove = -1.f;
//	}


	//float timestep = 1.f/60.f;
	// try to move character first vertically and then horizontally.
	// Not sure if this is better than the other way around.
	bool moved {false};
	if (_verticalMove != 0.f) {
		auto testVertical = _room->getPixel(_x, _y + _verticalMove);
		if (testVertical != 0) {
			_y += _verticalMove;
			moved = true;
		}
	}
	if (_horizontalMove != 0.f) {
		auto testHorizontal = _room->getPixel(_x + _horizontalMove, _y);
		if (testHorizontal != 0) {
			_x += _horizontalMove;
			moved = true;
		}
	}
//
//
//
//	auto test = _room->getPixel(_x + _horizontalMove, _y + _verticalMove);
//	if (test != 0) {
//		_x += _horizontalMove;// * _speed * timestep;
//		_y += _verticalMove;// * _speed * timestep;
//	}
	if (moved) {
		computePriority();
		//std::cout << _x <<", " << _y << ", " << _priority << "\n";
	}
	// animate
	auto px = _room->getPixel(1, 1);

	if (_dir == 0) {
		_model->setUpdate(false);
	} else {
		_model->setUpdate(true);
		switch (_dir) {
			case 2:
			case 3:
			case 4:
				_model->setAnimation("0");
				break;
			case 6:
			case 7:
			case 8:
				_model->setAnimation(_hasLeftAnim ? "1" : "0");
				break;
			case 1:
				if (_hasUpDownAnim) _model->setAnimation("3");
				break;
			case 5:
				if (_hasUpDownAnim) _model->setAnimation("2");
				break;
		}
	}

}