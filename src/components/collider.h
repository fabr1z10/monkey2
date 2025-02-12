#pragma once

#include "../component.h"
#include "../shape.h"

class Collider : public Component {

public:
    Collider(std::shared_ptr<Shape>);

    virtual ~Collider();

    void start() override;

    Shape* getShape();
private:
    std::shared_ptr<Shape> _shape;
};

inline Shape *Collider::getShape() {
    return _shape.get();
}