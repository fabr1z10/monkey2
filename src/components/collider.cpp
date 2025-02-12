#include "collider.h"
#include "../game.h"


void Collider::start() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
    engine->addCollider(this);
}

Collider::~Collider() {
    auto* engine = Game::instance().getRoom()->getCollisionEngine();
    engine->rmCollider(this);

}