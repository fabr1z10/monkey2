#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "component.h"

class IModel;

class IRenderer;

class Node {
public:
	Node();

	void add(std::shared_ptr<Node>);

	void start();

	void update(double);

	const std::vector<std::shared_ptr<Node>>& getChildren() const;

	glm::mat4 getWorldMatrix() const;

	void setModel(std::shared_ptr<IModel>, int batchId);

	void addComponent(std::shared_ptr<Component>);

	void setTransform(glm::mat4 t);
private:
    void notifyMove();
	std::vector<std::shared_ptr<Node>> _children;
	glm::mat4 _modelMatrix;
	glm::mat4 _worldMatrix;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<IModel> _model;
	std::vector<std::shared_ptr<Component>> _components;
	Node* _parent;
};

inline const std::vector<std::shared_ptr<Node>> & Node::getChildren() const {
	return _children;
}