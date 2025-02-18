#pragma once

#include "../script.h"
#include "../node.h"
#include "../adventure/walkarea.h"
#include "glm/glm.hpp"

namespace actions {
    class WalkTo : public Action {
    public:
        WalkTo(Node* node, adventure::WalkArea*, glm::vec2 position, float speed);

        void start() override;

        void run(double) override;
    private:
        void initialize();
        struct WalkSegment {
            glm::vec2 direction;
            float length;
            glm::vec2 targetPosition;
            bool flip;
            std::string anim;
        };
        // input
        Node* _node;
        adventure::WalkArea* _walkArea;
        glm::vec2 _targetPosition;
        float _speed;

        // internal
        float _distanceTraversed;
        std::string _lastDirection;
        int _segmentId;
        std::vector<WalkSegment> _segments;
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
