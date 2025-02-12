#pragma once

#include "../script.h"
#include "../node.h"
#include "glm/glm.hpp"

namespace actions {
    class MoveTo : public Action {
    public:
        MoveTo(Node* node, glm::vec2 position, float speed);

        void start() override;

        void run(double) override;
    private:
        Node* _node;

        glm::vec2 _targetPosition;

        float _speed;

        float _length;

        float _distanceTraversed;

        glm::vec2 _direction;
    };

    class FlipHorizontal : public Action {
    public:
        FlipHorizontal(Node*, bool value);

        void start() override;

        void run(double) override {}
    private:
        Node* _node;
        bool _value;
    };
}