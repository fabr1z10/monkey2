#pragma once

#include "../script.h"
#include <pybind11/pybind11.h>

namespace actions {

class CallFunc : public Action {
public:
    explicit CallFunc(pybind11::function f);

    void start() override;

    void run(double) override;
private:
    pybind11::function m_func;
};

}
