#include "npc.h"
#include <iostream>
#include "../util.h"


NPC::NPC(adventure::WalkArea* walkarea, float refreshTime, float speed, pybind11::function onRefresh)
    : Component(), _walkArea(walkarea), _refreshTime(refreshTime), _speed(speed), _onRefresh(onRefresh),_walkAction(nullptr)
{}

void NPC::start() {
    Component::start();
    refresh();
}

void NPC::update(double dt) {
    _timer += (float)dt;
    if (_timer >= _refreshTime) {
        refresh();
        std::cout << "REFRESHED\n";
    }
    if (_walkAction != nullptr) {
        if (_walkAction->isCompleted()) {
            _walkAction = nullptr;
            refresh();
        } else {
            _walkAction->run(dt);
        }
    }

}


void NPC::refresh() {
    auto obj = _onRefresh();
    if (!obj.is_none()) {
        auto pos = obj.cast<Vec2>();
        _walkAction = std::make_unique<actions::WalkTo>(m_node, _walkArea, pos, _speed);
        _walkAction->start();
    }
    _timer = 0.f;
    
}
