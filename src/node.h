#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "component.h"
#include "nodeobserver.h"

class IModel;

class IRenderer;

class Node {
public:
	Node();

    virtual ~Node();

    int id() const;

	void add(std::shared_ptr<Node>);

	void remove();

	virtual void start();

	virtual void update(double);



	const std::vector<std::shared_ptr<Node>>& getChildren() const;

	glm::mat4 getWorldMatrix() const;

    glm::vec3 getWorldPosition() const;

    void setPosition(glm::vec3);

    void move(glm::vec2);

    void flipHorizontal(bool);

	void setModel(std::shared_ptr<IModel>, int batchId);

	void addComponent(std::shared_ptr<Component>);

	void setTransform(glm::mat4 t);

    IRenderer* getRenderer();

    static Node* getNode(int);

	pybind11::object getUserData();

	void setUserData(pybind11::object);

	bool active() const;

	bool show() const;

	void setActive(bool);

	void setShow(bool);

	bool isMarkedForRemoval() const;

	void registerObserver(NodeObserver*);

	void unregisterObserver(NodeObserver*);

	void setAnimation(const std::string&);
private:
    void notifyMove();
	std::vector<std::shared_ptr<Node>> _children;
	glm::mat4 _modelMatrix;
	glm::mat4 _worldMatrix;
	std::shared_ptr<IRenderer> _renderer;
	std::shared_ptr<IModel> _model;
	std::vector<std::shared_ptr<Component>> _components;
	Node* _parent;
    int _id;
    static int _nextId;
    static std::unordered_map<int, Node*> _nodes;
	pybind11::object _userData;
	bool _active;
	bool _show;
	bool _toBeRemoved;
	std::vector<NodeObserver*> _observers;
};

inline const std::vector<std::shared_ptr<Node>> & Node::getChildren() const {
	return _children;
}

inline IRenderer *Node::getRenderer() {
    return _renderer.get();
}


inline int Node::id() const {

    return  _id;
}

inline pybind11::object Node::getUserData() {
	return _userData;
}

inline void Node::setUserData(pybind11::object data) {
	_userData = data;
}

inline bool Node::active() const {
	return _active;
}

inline bool Node::show() const {
	return _show;
}

inline void Node::setActive(bool value) {
	_active = value;
}


inline void Node::setShow(bool value) {
	_show = value;
}

inline bool Node::isMarkedForRemoval() const {
	return _toBeRemoved;
}