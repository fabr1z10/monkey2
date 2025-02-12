#pragma once

#include "../component.h"
#include "../camera.h"
#include <pybind11/pybind11.h>


class Follow : public Component {
public:
    Follow(int camId);

    void update(double) override;

    void start() override;
protected:
    glm::vec3 m_relativePos;
    glm::vec3 m_up;
    Camera* m_cam;
    glm::vec3 m_previous;
    int _camId;
};