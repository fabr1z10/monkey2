#pragma once

#include "pysubclassable.h"

class Node;

class Component : public PySubclassable {
public:
    Component() : _active(true), m_node(nullptr) {}
    virtual ~Component() = default;
    virtual void start() {}
    virtual void update(double) {}
    virtual void setNode(Node*);
    //virtual void dispose();
    Node* getNode();
    bool isActive() const;
    void setActive(bool);
    std::string getId() const;
    void setId(const std::string& id);
protected:
    Node* m_node;
    bool _active;
    std::string _id;

};

inline std::string Component::getId() const {
    return _id;
}

inline void Component::setId(const std::string& id) {
    _id = id;
}



inline Node* Component::getNode() {
    return m_node;
}

inline void Component::setNode(Node* node) {
    m_node = node;
}



inline bool Component::isActive() const {
    return _active;
}

inline void Component::setActive(bool value) {
    _active = value;
}


