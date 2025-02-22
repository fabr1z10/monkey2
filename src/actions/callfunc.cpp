#include "callfunc.h"

using namespace actions;

CallFunc::CallFunc(pybind11::function f) {
    m_func = std::move(f);
}


void CallFunc::start() {
    Action::start();
    m_func();
    _completed=true;

}

void CallFunc::run(double) {

}
