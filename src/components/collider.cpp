#include "collider.h"
#include "../game.h"


void Collider::start() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
    engine->addCollider(this);
}

Collider::Collider(std::shared_ptr<Shape> shape, int flag, int mask, const std::string& tag) : Component(),
	_shape(shape), _flag(flag), _mask(mask), _tag(tag) {}


Collider::~Collider() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
    engine->rmCollider(this);

}

Bounds Collider::getBounds() {
	auto b = _shape->getBounds();
	return b.transform(m_node->getWorldMatrix());
}