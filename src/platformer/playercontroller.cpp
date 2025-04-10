#include "playercontroller.h"
#include <iostream>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

PlayerController::PlayerController(float speed, float jumpSpeed) : Component(),
_speed(speed), _jumpSpeed(jumpSpeed) {

}

void PlayerController::start() {
	_collider = m_node->getComponent<TileCollider>();
	if (_collider == nullptr) {
		std::cerr << "PlayerController: no TileCollider found\n";
		return;
	}
}

void PlayerController::update(double dt) {
	int r = glfwGetKey(window, GLFW_KEY_RIGHT);
	int l = glfwGetKey(window, GLFW_KEY_LEFT);
	int u = glfwGetKey(window, GLFW_KEY_UP);
	if (r == GLFW_PRESS) {
		_collider->setVelocityX(_speed);
	} 
	else if (l == GLFW_PRESS) {
		_collider->setVelocityX(-_speed);
	}
	else {
		_collider->setVelocityX(0.f);
	}

	if (u == GLFW_PRESS && _collider->grounded()) {
		_collider->setVelocityY(_jumpSpeed);
	}
}
