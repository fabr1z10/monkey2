#include "waitclick.h"
#include <GLFW/glfw3.h>
using namespace actions;

int WaitForMouseClick::mouseButtonCallback(GLFWwindow *, int button, int action, int mods) {
	if (_started && button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS) {
		_f();
		_completed = true;
		return 0;
	}
	return 1;

}


void WaitForMouseClick::start() {
	Action::start();
	_g();
	_started = true;
}