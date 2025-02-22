#pragma once


#include "../component.h"
#include "../shape.h"
#include <pybind11/pybind11.h>

class HotSpot : public Component {

public:
    HotSpot(std::shared_ptr<Shape> shape, int);

    void start() override;


    virtual ~HotSpot();

    void setOnEnter(pybind11::function f);

    void setOnLeave(pybind11::function f);

    void setOnClick(pybind11::function f);

    void onEnter() {}

    void onLeave(){}

    void onClick(){}

    int getPriority() const;

private:
    int _priority;
    std::shared_ptr<Shape> _shape;
    pybind11::function _onEnter;
    pybind11::function _onLeave;
    pybind11::function _onClick;
};


inline int HotSpot::getPriority() const {
    return _priority;
}
