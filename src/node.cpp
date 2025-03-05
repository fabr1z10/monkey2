#include "node.h"
#include "renderer.h"
#include "model.h"
#include <glm/gtx/transform.hpp>

int Node::_nextId = 0;

std::unordered_map<int, Node*> Node::_nodes;

Node::Node() : _id(Node::_nextId++), _renderer(nullptr), _parent(nullptr), _active(true),
	_show(true), _toBeRemoved(false) {
	_modelMatrix = glm::mat4(1.f);
	_worldMatrix = glm::mat4(1.f);
    _nodes[_id] = this;
	_userData = pybind11::none();
}

Node::~Node() {
	std::cout << " ** removed node " << _id << "\n";
	for (auto& obs : _observers) {
		obs->onRemove(this);
	}
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
	for (auto& obs : _observers) {
		obs->onMove(this);
	}
}

void Node::update(double dt) {
	// first remove all nodes marked as removal
	_children.erase(std::remove_if(_children.begin(), _children.end(), [](const std::shared_ptr<Node>& node)
		{ return node->isMarkedForRemoval(); }), _children.end());


    for (auto& c : _components) {
        c->update(dt);
    }
	// note that wehn node is inactive, render also should not be performed!
	// in fact, renderer updates position / frame, and if node is not
	// active, we don't want that.
	// node update is not recursive!ma s
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

void Node::remove() {
	_toBeRemoved = true;
}


void Node::addComponent(std::shared_ptr<Component> c) {
    _components.push_back(c);
    c->setNode(this);
	// Extract Python object if this is a Python-derived class
	// Try to preserve the Python object using a different approach
	try {
		// Acquire the GIL
		py::gil_scoped_acquire gil;

		// This will either find the existing Python object or create a new wrapper
		py::object py_obj = py::cast(c);

		// Store the Python object reference in the component
		c->setPySelf(py_obj);

		std::cout << "Python object reference stored in component" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Failed to create Python reference: " << e.what() << std::endl;
	}


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


void Node::registerObserver(NodeObserver * observer) {
	_observers.push_back(observer);

}

void Node::unregisterObserver(NodeObserver * observer) {
	_observers.erase(
			std::remove(_observers.begin(), _observers.end(), observer),
			_observers.end()
	);
}

void Node::setAnimation(const std::string & anim) {
	_renderer->setAnimation(anim);
}