#include "hotspot.h"
#include "../game.h"
#include "../adventure/mousecontroller.h"
#include "../util.h"
// a hotspot is an area with mouse interaction
// you can define events for:
// mouse entering the area, mouse leaving area, mouse clicking


HotSpot::HotSpot(std::shared_ptr<Shape> shape, int priority, int cam) : Component(),
	_shape(shape), _priority(priority), _cam(cam)
{

}


void HotSpot::start()
{
    auto hsm = dynamic_cast<adventure::MouseController*>(Game::instance().getRoom()->getHotSpotManager());
    hsm->add(this);
}

HotSpot::~HotSpot() {
    auto hsm = dynamic_cast<adventure::MouseController*>(Game::instance().getRoom()->getHotSpotManager());
    hsm->remove(this);


}



bool HotSpot::isInside(glm::vec2 worldCoords) {
	auto local = glm::vec2(glm::inverse(m_node->getWorldMatrix()) * glm::vec4(worldCoords, 0.f, 1.f));
	return _shape->isInside(local);
}