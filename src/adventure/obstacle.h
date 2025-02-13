#pragma once

#include "../node.h"

class Obstacle : public Node {

public:
    Obstacle(const std::string& texture, int x, int y, int w, int h, float threshold);
private:
    std::vector<glm::vec2> simplifyOutline(const std::vector<glm::vec2>&);
    float _threshold;
};
