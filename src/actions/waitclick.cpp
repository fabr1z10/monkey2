#include "waitclick.h"
#include <GLFW/glfw3.h>
using namespace actions;

void WaitForMouseClick::mouseButtonCallback(GLFWwindow *, int button, int action, int mods) {
	if (_started && button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS) {
		_completed = true;
	}

}


void WaitForMouseClick::start() {
	Action::start();
	_started = true;
}