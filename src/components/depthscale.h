#pragma once

#include "../component.h"
#include "../collisionengine.h"
#include "../shapes/line.h"

class DepthScale : public Component {
public:
    DepthScale(float y0, float y1, int mask);

    void start() override;

    void update(double) override;
private:
    void updateZ();
	int _mask;
    ICollisionEngine* _engine;
    glm::vec2 _lastPos;
    float _y0, _y1, k;
    Collider* _collider;
	shapes::PolyLine* _poly;
    bool _valid;
};
