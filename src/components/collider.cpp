#include "collider.h"
#include "../game.h"
#include "../models/linemodel.h"


Collider::Collider(std::shared_ptr<Shape> shape, int flag, int mask, const std::string& tag, int batchId) :
	Component(), _shape(shape), _flag(flag), _mask(mask), _tag(tag), _batchId(batchId) {

}


void Collider::start() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
	M_Assert(engine != nullptr, "Collider component requires a collision engine object in the room.");
    engine->addCollider(this);

	if (_batchId != -1) {
		auto model = _shape->makeModel(_batchId,ModelType::WIREFRAME);
		auto node = std::make_shared<Node>();
		node->setModel(model);
		m_node->add(node);
	}
}




Collider::~Collider() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
    engine->rmCollider(this);

}

Bounds Collider::getBounds() {
	auto b = _shape->getBounds();
    if (m_node == nullptr) {
        return b;
    }
	return b.transform(m_node->getWorldMatrix());
}

AABBCollider::AABBCollider(int flag, int mask, int tag, float width, float height, Vec2 anchor, int batch) : Component(),
	_flag(flag), _mask(mask), _tag(tag), _batchId(batch)
{
	_aabb.xm = -anchor.x;
	_aabb.xM = -anchor.x + width;
	_aabb.ym = -anchor.y;
	_aabb.yM = -anchor.y + height;

}


void AABBCollider::start() {
	// add to engine


	// draw debug shape if requested

}