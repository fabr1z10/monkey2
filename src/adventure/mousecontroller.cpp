#include "mousecontroller.h"
#include <iostream>
#include "../game.h"
#include "walkarea.h"
#include "../util.h"
#include "../actions/walk.h"
#include "../renderer.h"
#include "../pyhelper.h"

using namespace adventure;

extern GLFWwindow * window;

// mouse controller first checks on which camera we are on
MouseController::MouseController(float zCursor) :
    Node(), MouseListener(), _zCursor(zCursor), _previous(nullptr), _cursor(nullptr) {

}

void MouseController::start() {
	Node::start();
	_room = Game::instance().getRoom();
	_cameraCount = _room->getCameraCount();
	for (int i = 0; i < _cameraCount; ++i) {
		CameraInfo info;
		info.cam = dynamic_cast<OrthoCamera*>(_room->getCamera(i));
		info.viewport = info.cam->getViewport();
		info.active = true;
		_cam.push_back(info);
		if (_hotSpots.find(i) == _hotSpots.end()) {
			_hotSpots[i] = std::map<int, std::unordered_set<HotSpot*>>();
		}

	}
	_cursorType = 0;
	if (_onRightClick) {
        _onRightClick(_cursorSequences[_cursorSeq][_cursorType]);
	}
}

void MouseController::cursorPosCallback(GLFWwindow*, double x, double y) {
    // first get device coordinates
    if (_cursor != nullptr) {
        auto devCoords = Game::instance().getDeviceCoordinates({x, y});
        _cursor->setPosition(Vec3(devCoords.x, devCoords.y, _zCursor));
    }


    //glm::vec2 worldCoords;
	_currentCameraId = -1;
    if (int cam = screenCoordsToWorldCoords({x, y}, _worldCoords); cam >= 0) {
		_currentCameraId = cam;
        // check hotspot
		bool handled=false;
		//std::cout << //" camera = " << cam << "\n";
		for (const auto&[priority, s] : _hotSpots.at(cam)) {
			for (const auto& h : s) {
				if (h->isActive() && h->isInside(_worldCoords)) {
					// if I'm here --> we can break and handle the enter
					if (_previous != h) {
						if (_previous != nullptr) {
							_previous->onLeave();
						}
						h->onEnter();
						_previous = h;
					}
					handled=true;
					break;
				}
			}
			if (handled) break;
		}
		if (!handled && _previous != nullptr) {
			_previous->onLeave();
			_previous = nullptr;
		}
    }
}


int MouseController::screenCoordsToWorldCoords(glm::vec2 screenCoords, glm::vec2& worldCoords) const {
	// loop through all cameras
	auto devCoords = Game::instance().getDeviceCoordinates(screenCoords);
	for (int i = 0; i < _cameraCount; ++i) {
		auto& cam = _cam[i];
		if (!cam.active) continue;
		bool isInViewport = devCoords.x >= cam.viewport.x && devCoords.x <= cam.viewport.x + cam.viewport[2] &&
							devCoords.y >= cam.viewport.y && devCoords.y <= cam.viewport.y + cam.viewport[3];
		if (isInViewport) {
			worldCoords = cam.cam->getWorldCoordinates(devCoords);
			//std::cout << "cursor in cam " << i << ", pos = " << worldCoords << "\n";
			return i;
		}
	}
	return -1;

}
int MouseController::mouseButtonCallback(GLFWwindow*, int button, int action, int mods) {
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
        while (true) {
            _cursorType++;
            auto& seq = _cursorSequences[_cursorSeq];
            if (_cursorType >= seq.size()) {
                _cursorType = 0;
            }
            if (!seq[_cursorType].empty()) {
				// TODO restore set animation
                //_cursor->getRenderer()->setAnimation(seq[_cursorType]);
                if (_onRightClick) {
                    _onRightClick(seq[_cursorType]);
                }
                break;
            }
        }
		return 0;
    }

    if (active() && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

		// if we are hovering on a hotspot ... call onClick
		if (_previous != nullptr) {
            _previous->onClick(Vec2(_worldCoords.x, _worldCoords.y));
		} else {
			// if we click outside a hotspot, we call onClick
			if (_onClick) {

				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				glm::vec2 worldCoords;
				if (int camId = screenCoordsToWorldCoords({xpos, ypos}, worldCoords); camId != -1) {
					std::cout << "Clicked at " << worldCoords << "\n";
                    _onClick(camId, Vec2(worldCoords.x, worldCoords.y));//, _cursorSequences[_cursorSeq][_cursorType]);
					//auto script = std::make_shared<Script>("__PLAYER");
					//script->addAction(std::make_shared<actions::WalkTo>(_player, _walkarea, worldCoords, _speed));
					//_scheduler->play(script);
				}
			}
		}
		return 0;
    }
	return 1;
}

void MouseController::setCursor(Node* node) {
    _cursor = node;
    _cursorSeq = 0;
    //_cursorSequence = seq;
    _cursorType = 0;
}

void MouseController::addCursorSequence(const std::vector<std::string>& seq) {
    _cursorSequences.push_back(seq);
}

void MouseController::setCursorAction(int seq, int index, const std::string& action) {
    _cursorSequences[seq][index] = action;
}

void MouseController::setSequence(int seq, int index) {
    _cursorSeq = seq;
    _cursorType = index;
    //_cursor->getRenderer()->setAnimation(_cursorSequences[seq][_cursorType]);

}

void MouseController::add(HotSpot * hs)
{
    _hotSpots[hs->getCamera()][hs->getPriority()].insert(hs);
}

void MouseController::remove(HotSpot* hs) {
    _hotSpots[hs->getCamera()][hs->getPriority()].erase(hs);
    if (_previous == hs) {
        _previous = nullptr;
    }
}



void MouseController::setOnClick(pybind11::function f)
{
	_onClick = f;
}

void MouseController::setOnRightClick(pybind11::function f)
{
	_onRightClick = f;
}

void MouseController::activateCamera(int camId, bool value) {
	_cam[camId].active = value;
	if (!value && _currentCameraId == camId && _previous) {
		_currentCameraId = -1;
		_previous = nullptr;
	}
}
