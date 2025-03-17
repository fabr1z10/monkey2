#pragma once

#include "../component.h"
#include "../adventure/walkarea.h"
#include "../actions/walk.h"

class NPC: public Component {
public:
    NPC(adventure::WalkArea* walkarea, float refreshTime, pybind11::function onRefresh, pybind11::function onReach);
    
    void start() override;

    void update(double) override;
private:
    void refresh();

    adventure::WalkArea* _walkArea;

    std::unique_ptr<actions::WalkTo> _walkAction;

    pybind11::function _onRefresh;

    pybind11::function _onReach;

    float _timer;

    float _refreshTime;

    std::vector<glm::vec2> _path;

    int _segIndex;
};
