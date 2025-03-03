#pragma once

#include "../node.h"
#include "../camera.h"
#include "../mouselistener.h"
#include "../components/hotspot.h"
#include "../scheduler.h"
#include <map>

class Room;

namespace adventure {
    class WalkArea;

    class MouseController : public Node, public MouseListener {
    public:
        MouseController(WalkArea*,Node*, Scheduler* scheduler, float speed);

        void start() override;

        void cursorPosCallback(GLFWwindow *, double, double) override;

        void mouseButtonCallback(GLFWwindow *, int, int, int) override;

        void setCursor(Node*, const std::vector<std::string>& seq);

        void add(HotSpot*);

        void remove(HotSpot*);

		void setOnClick(pybind11::function f);

		void setOnRightClick(pybind11::function f);

		void activateCamera(int, bool);
    private:
		struct CameraInfo {
			OrthoCamera* cam;
			glm::vec4 viewport;
			bool active;
		};
        int screenCoordsToWorldCoords(glm::vec2 screenCoords, glm::vec2& worldCoords) const;
        WalkArea *_walkarea;
        Node* _player;
        Node* _cursor;
        Scheduler* _scheduler;
        int _camId;
        std::vector<CameraInfo> _cam;
        float _speed;
        std::vector<std::string> _cursorSequence;
        int _cursorType;
        std::unordered_map<int, std::map<int, std::unordered_set<HotSpot*>>> _hotSpots;
		HotSpot* _previous;
		pybind11::function _onEnter;
		pybind11::function _onLeave;
		pybind11::function _onClick;
		pybind11::function _onRightClick;
		Room* _room;
		int _cameraCount;
		int _currentCameraId;
		glm::vec2 _worldCoords;
    };


}

