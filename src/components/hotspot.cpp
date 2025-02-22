#include "hotspot.h"
#include "../game.h"
#include "../adventure/mousecontroller.h"
// a hotspot is an area with mouse interaction
// you can define events for:
// mouse entering the area, mouse leaving area, mouse clicking


HotSpot::HotSpot(std::shared_ptr<Shape> shape, int priority) : Component(), _shape(shape), _priority(priority)
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

void HotSpot::setOnEnter(pybind11::function f)
{
    _onEnter = f;
}

void HotSpot::setOnLeave(pybind11::function f)
{
    _onLeave = f;
}

void HotSpot::setOnClick(pybind11::function f)
{
    _onClick = f;
}


