#include "move.h"

using namespace actions;

// FlipHorizontal::FlipHorizontal(Node * node, bool value) : Action(), _node(node), _value(value) {

// }

// void FlipHorizontal::start() {
//     _node->flipHorizontal(_value);
//     _completed = true;
// }

MoveTo::MoveTo(Node* node, Vec2 position, float speed) : Action(), _node(node), _targetPosition(position.toGlm()),
    _speed(speed) {}


void MoveTo::start() {
    Action::start();
    auto pos = glm::vec2(_node->getWorldPosition().toGlm());

    _direction = glm::normalize(_targetPosition - pos);
    _length = glm::length(_targetPosition - pos);
    _distanceTraversed = 0.f;

}



void MoveTo::run(double dt) {
    float ds = (float)dt * _speed;
    auto delta = ds * _direction;
    _node->move(delta);
    _distanceTraversed += ds;
    if (_distanceTraversed >= _length) {
        _node->setPosition(Vec3(_targetPosition.x, _targetPosition.y, 0.f));
        _completed = true;
    }
}
