#include "tilecontroller.h"
#include "../model.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;


TileController::TileController(float width, float height, int batch) : Component(), 
	_width(width), _height(height), _batchId(batch),_velocity(glm::vec2(0.f)) {
}	

void TileController::start() {
	float hw = _width * 0.5f;
	std::vector<float> debugModelData{
		-hw, 0.f, 0.f, hw, 0.f, 0.f,
		hw, 0.f, 0.f, hw, _height, 0.f,
		hw, _height, 0.f, -hw, _height, 0.f,
		-hw, _height, 0.f, -hw, 0.f, 0.f
	};
	auto model = std::make_shared<Model<primitives::Line>>(debugModelData);
	m_node->setModel(model, _batchId);
	_world = dynamic_cast<TileWorld*>(m_node->getParent());
	if (_world == nullptr) {
		std::cerr << "TileController: parent is not a TileWorld\n";
		return;
	}
}


void TileController::update(double dt) {
	auto pos = m_node->getWorldPosition();
	auto delta = 0.0167f * _velocity;
	auto result = _world->ver(pos.x, pos.y, pos.y + delta.y);
	float newY = pos.y;
	if (result.collide) {
		_velocity.y = 0.f;
		m_node->move(glm::vec2(0.f, result.position - pos.y));
		newY = result.position;
	}
	else {
		m_node->move(glm::vec2(0.f, delta.y));
		newY = pos.y + delta.y;
	}
	if (delta.x != 0.f) {
		result = _world->hor(pos.x, newY, pos.x + delta.x);
		if (result.collide) {
			m_node->move(glm::vec2(result.position - pos.x, 0.f));
		}
		else {
			m_node->move(glm::vec2(delta.x, 0.f));
		}
	}


	_velocity.y -= 10.f * (float)dt;

	int state = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS) {
		_velocity.x = 5.f;
	}
	else {
		_velocity.x = 0.f;
	}

}
