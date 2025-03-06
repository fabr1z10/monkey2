#include "callfunc.h"

using namespace actions;

CallFunc::CallFunc(const pybind11::function& f) {
    m_func = f;
}


void CallFunc::start() {
    Action::start();
    m_func();
    _completed=true;

}

void CallFunc::run(double) {

}
