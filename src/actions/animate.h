#pragma once

#include "../script.h"
#include "../node.h"

namespace actions {


    class Animate : public Action {
    public:
        Animate(Node* node, const std::string& animation);

        void start() override;

        void run(double) override {}
    private:
        Node* _node;
        std::string _animation;
    };
}