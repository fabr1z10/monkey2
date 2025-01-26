#include "camcontrol3d.h"
#include "../game.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>


extern GLFWwindow * window;

CamControl3D::CamControl3D(int camera, float speed, float rotationSpeed) :
    Component(), _camId(camera), _speed(speed), _rotationSpeed(rotationSpeed) {

}

void CamControl3D::start() {
    _cam = Game::instance().getRoom()->getCamera(_camId);
}


void CamControl3D::update(double dt) {
    float dtf = static_cast<float>(dt);
    bool up = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    bool left = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    auto fwd = _cam->getFwd();
    if (up) {
        _cam->move(fwd * dtf * _speed);
    } else if (down) {
        _cam->move(-fwd * dtf * _speed);
    }

    if (left) {
        auto t = glm::rotate(_rotationSpeed* dtf, glm::vec3(0,1,0));
        auto f = glm::vec3(t * glm::vec4(fwd, 0.f));
        _cam->setFwd(f);
    } else if(right) {
        auto t = glm::rotate(-_rotationSpeed* dtf, glm::vec3(0,1,0));
        auto f = glm::vec3(t * glm::vec4(fwd, 0.f));
        _cam->setFwd(f);

    }


}