#include "mousecontroller.h"
#include <iostream>
#include "../game.h"
#include "walkarea.h"
#include "../util.h"
#include "../actions/walk.h"
#include "../renderer.h"

using namespace adventure;

extern GLFWwindow * window;

MouseController::MouseController(int camId, WalkArea* walkarea, Node* player, Scheduler* scheduler, float speed) :
    Node(), MouseListener(), _camId(camId), _walkarea(walkarea), _player(player), _scheduler(scheduler), _speed(speed), _cursor(nullptr) {
}

void MouseController::start() {
    _cam = dynamic_cast<OrthoCamera*>(Game::instance().getRoom()->getCamera(_camId));
    _camViewport = _cam->getViewport();
}

void MouseController::cursorPosCallback(GLFWwindow*, double x, double y) {

    // first get device coordinates
    if (_cursor != nullptr) {
        auto devCoords = Game::instance().getDeviceCoordinates({x, y});
        //std::cout << "pipa:" << devCoords << "\n";
        _cursor->setPosition(glm::vec3(devCoords, 10.f));

    }


    glm::vec2 worldCoords;
    if (screenCoordsToWorldCoords({x, y}, worldCoords)) {
        // check hotspot

    }
}


bool MouseController::screenCoordsToWorldCoords(glm::vec2 screenCoords, glm::vec2& worldCoords) const {
    auto devCoords = Game::instance().getDeviceCoordinates(screenCoords);
    bool isInViewport = devCoords.x >= _camViewport.x && devCoords.x <= _camViewport.x + _camViewport[2] &&
                        devCoords.y >= _camViewport.y && devCoords.y <= _camViewport.y + _camViewport[3];
    if (isInViewport) {
        worldCoords = _cam->getWorldCoordinates(devCoords);
        //std::cout << "ora in: " << worldCoords << "\n";
        return true;
    }
    return false;

}
void MouseController::mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
    /* When the user clicks the left button, the default callback
     * first checks if cursor is positioned on a mouse
     * hotspot. In this case the hotspot callback will be called.
     * If not, we assume we are walking to the position clicked.
     * The walkarea now checks to which polygon the player is upon.
     * It also checks the polygon containing the target point.
     * If they don't belong to the same polygon, the target point is
     * assumed to be the closest point to the target point in the player poly.
     * The pathfinding algo is then run to determine the path to the
     * target point.
     */
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS && _cursor != nullptr) {
        _cursorType++;
        if (_cursorType >= _cursorSequence.size()) {
            _cursorType = 0;
        }
        _cursor->getRenderer()->setAnimation(_cursorSequence[_cursorType]);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 worldCoords;
        if (screenCoordsToWorldCoords({xpos, ypos}, worldCoords)) {
            std::cout << "Clicked at " << worldCoords << "\n";
            auto script = std::make_shared<Script>("__PLAYER");
            script->addAction(std::make_shared<actions::WalkTo>(_player, _walkarea, worldCoords, _speed));
            _scheduler->add(script);

//            auto pos = _player->getWorldPosition();
//            auto path = _walkarea->dijkstraShortestPath(glm::vec2(pos), worldCoords);
//            for (size_t i = 0; i < path.size(); i++) {
//                std::cout << path[i] << (i == path.size() - 1 ? "\n" : "->");
//            }
//            if (!path.empty()) {
//                auto script = std::make_shared<Script>("__PLAYER");
//                int j = -1;
//                std::string dir;
//                for (size_t i = 1; i < path.size(); ++i) {
//                    glm::vec2 increment = path[i] - path[i-1];
//                    dir = (increment.y >= fabs(increment.x) ? "-n" : (increment.y <= -fabs(increment.x) ? "-s" : "-e"));
//                    auto goingRight = (path[i].x - path[i-1].x) >= 0.f;
//                    script->addAction(std::make_shared<actions::FlipHorizontal>(_player, !goingRight), j++);
//                    script->addAction(std::make_shared<actions::Animate>(_player, "walk" + dir), j++);
//                    script->addAction(std::make_shared<actions::MoveTo>(_player, path[i], _speed), j++);
//                }
//                script->addAction(std::make_shared<actions::Animate>(_player, "idle" + dir), j++);
//
//                _scheduler->add(script);
//            }

        }
    }

}

void MouseController::setCursor(Node* node, const std::vector<std::string>& seq) {
    _cursor = node;
    _cursorSequence = seq;
    _cursorType = 0;
}


void MouseController::add(HotSpot * hs)
{
    _hotSpots[hs->getPriority()].insert(hs);
}

void MouseController::remove(HotSpot* hs) {
    _hotSpots[hs->getPriority()].erase(hs);
}
