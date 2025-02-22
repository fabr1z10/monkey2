#pragma once

#include "../script.h"

namespace actions {

class Delay : public Action {
public:
    explicit Delay(float t);

    void start() override;

    void run(double) override;
private:
    float m_time;
    float m_timer;
};

}
