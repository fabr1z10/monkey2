#include "node.h"
#include "renderer.h"
#include "model.h"


Node::Node() : _renderer(nullptr) {
	_modelMatrix = glm::mat4(1.f);
	_worldMatrix = glm::mat4(1.f);
}

void Node::start() {
	for (auto& c : _components) {
	    c->start();
	}
    if (_renderer != nullptr) {
		_renderer->start();
	}

}



void Node::update(double dt) {
    for (auto& c : _components) {
        c->update(dt);
    }
	// node update is not recursive!
	if (_renderer != nullptr) {
		_renderer->update();
	}
}

glm::mat4 Node::getWorldMatrix() const {
	return _worldMatrix;
}

void Node::setModel(std::shared_ptr<IModel> model, int batchId) {
	_model = model;
	_renderer = model->getRenderer(batchId);
	_renderer->setNode(this);

}

void Node::add(std::shared_ptr<Node> node) {
	_children.push_back(node);
}

void Node::addComponent(std::shared_ptr<Component> c) {
    _components.push_back(c);
}