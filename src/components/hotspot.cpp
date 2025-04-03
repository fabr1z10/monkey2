#include "hotspot.h"
#include "../game.h"
#include "../adventure/mousecontroller.h"
#include "../util.h"
// a hotspot is an area with mouse interaction
// you can define events for:
// mouse entering the area, mouse leaving area, mouse clicking


HotSpot::HotSpot(std::shared_ptr<Shape> shape, int priority, int cam, int batchId) : Component(),
	_shape(shape), _priority(priority), _cam(cam), _batchId(batchId), _debugDrawNode(nullptr)
{

}

void HotSpot::setShape(std::shared_ptr<Shape> shape) {
    _shape = shape;
    redraw();

}

void HotSpot::start()
{
    auto hsm = dynamic_cast<adventure::MouseController*>(Game::instance().getRoom()->getHotSpotManager());
    hsm->add(this);
    redraw();
}

HotSpot::~HotSpot() {
    //auto hsm = dynamic_cast<adventure::MouseController*>(Game::instance().getRoom()->getHotSpotManager());
    //hsm->remove(this);


}
void HotSpot::redraw() {

    if (_batchId != -1) {
        if (_debugDrawNode != nullptr) {
            _debugDrawNode->remove();
        }
        auto node = std::make_shared<Node>();
        auto model = _shape->makeModel(ModelType::WIREFRAME);
        node->setModel(model, _batchId);
        getNode()->add(node);
        _debugDrawNode = node.get();
    }
}


void HotSpot::dispose() {

    auto hsm = dynamic_cast<adventure::MouseController*>(Game::instance().getRoom()->getHotSpotManager());
    hsm->remove(this);
    Component::dispose();
}



bool HotSpot::isInside(glm::vec2 worldCoords) {
	auto local = glm::vec2(glm::inverse(m_node->getWorldMatrix()) * glm::vec4(worldCoords, 0.f, 1.f));
	return _shape->isInside(local);
}