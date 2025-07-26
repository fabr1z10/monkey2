#include "agiobject.h"
#include "agiroom.h"
#include "../game.h"
#include "../linebatch.h"

extern GLFWwindow* window;

using namespace agi;

AGIObject::AGIObject(const std::string &viewId, int x, int y, float speed) : Node(), _x(x), _y(y), _horizontalMove(0), _verticalMove(0),
_refresh(5), _ticks(0), _accumulatedTime(0.), _speed(speed) {

	_room = dynamic_cast<agi::AGIRoom*>(Game::instance().getRoom());
	auto* batch = dynamic_cast<QuadBatchPalette*>(_room->getBatch(1));
	auto model = batch->getSpriteSheet()->makeSprite(1, viewId);
	this->setModel(model);

	computePriority();
	//this->setPosition(Vec3(_x, _y, 0.f));



}

void AGIObject::computePriority() {
	int yt = 168 - static_cast<int>(_y);
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


	_verticalMove = 0.f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		_verticalMove = 1.f;
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		_verticalMove = -1.f;
	}
	_horizontalMove = 0.f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		_horizontalMove = 1.f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		_horizontalMove = -1.f;
	}

	float timestep = 1.f/60.f;
	_x += _horizontalMove;// * _speed * timestep;
	_y += _verticalMove;// * _speed * timestep;
	if (_horizontalMove != 0 || _verticalMove != 0) {
		computePriority();
	}

}