#pragma once

#include "glm/glm.hpp"
#include "light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color, float ambient);

    void setup(Shader*) override;

private:
    glm::vec3 _direction;
    glm::vec3 _color;
    float _ambient;
};