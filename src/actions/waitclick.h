#pragma once


#include "../script.h"
#include "../mouselistener.h"

namespace actions {
	class WaitForMouseClick : public Action, public MouseListener {

	public:
		WaitForMouseClick() : Action(), MouseListener(), _started(false) {}

		void start() override;

		void run(double) override {}

		void cursorPosCallback(GLFWwindow *, double, double) override {}

		void mouseButtonCallback(GLFWwindow *, int, int, int) override;
	private:
		bool _started;
	};
}