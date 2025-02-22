#include "delay.h"

using namespace actions;

Delay::Delay(float t) : Action(), m_time(t), m_timer(0.f) {}

void Delay::start() {
    Action::start();
    m_timer = 0.f;
}

void Delay::run(double dt) {
    m_timer += static_cast<float>(dt);
    if (m_timer > m_time) {
        _completed = true;
    }
}
