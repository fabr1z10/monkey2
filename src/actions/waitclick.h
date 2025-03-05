#pragma once


#include "../script.h"
#include "../mouselistener.h"
#include <pybind11/pybind11.h>

namespace actions {
	class WaitForMouseClick : public Action, public MouseListener {

	public:
		WaitForMouseClick(pybind11::function f, pybind11::function g) : Action(), MouseListener(), _started(false), _f(f), _g(g) {}

		void start() override;

		void run(double) override {}

		void cursorPosCallback(GLFWwindow *, double, double) override {}

		int mouseButtonCallback(GLFWwindow *, int, int, int) override;
	private:
		bool _started;
		pybind11::function _f;
		pybind11::function _g;
	};
}