#pragma once

#include "../component.h"

class Camera;

class CamControl3D : public Component {
public:
    CamControl3D(int camera, float speed, float rotationSpeed);
    void start() override;
    void update(double) override;
private:
    float _speed;
    float _rotationSpeed;
    int _camId;
    Camera* _cam;
};