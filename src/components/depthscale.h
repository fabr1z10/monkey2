#pragma once

#include "../component.h"
#include "../collisionengine.h"


class DepthScale : public Component {
public:
    DepthScale(float y0, float y1);

    void start() override;

    void update(double) override;
private:
    void updateZ();
    CollisionEngine* _engine;
    glm::vec2 _lastPos;
    float _y0, _y1, k;
};