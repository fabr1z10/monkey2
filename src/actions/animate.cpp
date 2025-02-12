#include "animate.h"
#include "../renderer.h"


using namespace actions;


Animate::Animate(Node *node, const std::string &animation) : Action(), _node(node), _animation(animation) {}


void Animate::start() {
    _node->getRenderer()->setAnimation(_animation);
    _completed = true;
}