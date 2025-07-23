#include "controller.h"
#include "../node.h"
#include "../error.h"
#include "../game.h"

extern GLFWwindow* window;


Controller::Controller(float speed, int mask) : Component(), _speed(speed), _mask(mask) {

}

void Controller::start() {
	_engine = Game::instance().getRoom()->getCollisionEngine();
	M_Assert(_engine != nullptr, "Controller component requires a collision engine!");
}


void Controller::update(double dt) {
	bool left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	bool up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	bool down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;

	glm::vec2 displacement;
	displacement.x = left ? -1.f : (right ? 1.f : 0.f);
	displacement.y = down ? -1.f : (up ? 1.f : 0.f);
	if (left) {
		m_node->flipHorizontal(true);
	} else if (right) {
		m_node->flipHorizontal(false);
	}
	displacement *= _speed;

	auto pos = glm::vec2(m_node->getWorldPosition().toGlm());

	if (displacement.x != 0.f) {
		auto result = _engine->raycastX(pos, displacement.x, _mask, m_node, true);
		if (result.hit) {
			displacement.x = decrementTowardZero(result.length, 0.01f);
		}
	}
	if (displacement.y != 0.f) {
		auto result = _engine->raycastY({pos.x + displacement.x, pos.y}, displacement.y, _mask, m_node, true);
		if (result. hit) {
			displacement.y = decrementTowardZero(result.length, 0.01f);
			std::cout << pos.y << ", " << displacement.y << "\n";
		}
	}
	m_node->move(displacement);


}