#include "node.h"
#include "renderer.h"
#include "model.h"
#include <glm/gtx/transform.hpp>

int Node::_nextId = 0;

std::unordered_map<int, Node*> Node::_nodes;

Node::Node() : _id(Node::_nextId++), _renderer(nullptr), _parent(nullptr) {
	_modelMatrix = glm::mat4(1.f);
	_worldMatrix = glm::mat4(1.f);
    _nodes[_id] = this;
	_userData = pybind11::none();
}

Node::~Node() {
    _nodes.erase(_id);
}

void Node::start() {
	for (auto& c : _components) {
	    c->start();
	}
    if (_renderer != nullptr) {
		_renderer->start();
	}

}

void Node::setTransform(glm::mat4 m) {
    _modelMatrix = m;
    notifyMove();
}

void Node::notifyMove() {
    if (_parent != nullptr) {
        _worldMatrix = _parent->getWorldMatrix() * _modelMatrix;
    } else {
        _worldMatrix = _modelMatrix;
    }
    if (_renderer) {
        _renderer->updateGeometry();
    }
    for (auto& child : _children) {
        child->notifyMove();
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

glm::vec3 Node::getWorldPosition() const {
    return glm::vec3(_worldMatrix[3]);
}

void Node::setModel(std::shared_ptr<IModel> model, int batchId = -1) {
	_model = model;
	_renderer = model->getRenderer(batchId);
	_renderer->setNode(this);
    if (Game::instance().started()) {
        _renderer->start();
    }

}

void Node::add(std::shared_ptr<Node> node) {
	_children.push_back(node);
	node->_parent = this;
	node->notifyMove();
}

void Node::addComponent(std::shared_ptr<Component> c) {
    _components.push_back(c);
    c->setNode(this);
}

void Node::move(glm::vec2 dx) {
    _modelMatrix = glm::translate(glm::vec3(dx, 0.f)) * _modelMatrix;
    notifyMove();
}

void Node::setPosition(glm::vec3 pos) {
    _modelMatrix[3] = glm::vec4(pos, 1.f);
    notifyMove();
}

void Node::flipHorizontal(bool value) {
    _modelMatrix[0][0] = fabs(_modelMatrix[0][0]) * (value ? -1.0f : 1.0f);
    notifyMove();
}

Node* Node::getNode(int id) {
    return Node::_nodes.at(id);
}
