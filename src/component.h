#pragma once

#include <pybind11/pybind11.h>

class Node;

class Component {
public:
    Component() : _active(true) {}
    virtual ~Component() = default;
    virtual void start() {}
    virtual void update(double) {}
    virtual void setNode(Node*);
    virtual void dispose();
    Node* getNode();
    bool isActive() const;
    void setActive(bool);
	void setPySelf(pybind11::object self);
protected:
    Node* m_node;
    bool _active;
	pybind11::object _py_self;
};

inline void Component::dispose() {
    _py_self = pybind11::none();
}
inline Node* Component::getNode() {
    return m_node;
}

inline void Component::setNode(Node* node) {
    m_node = node;
}

inline void Component::setPySelf(pybind11::object self) {
	_py_self = self;
}


inline bool Component::isActive() const {
    return _active;
}

inline void Component::setActive(bool value) {
    _active = value;
}


