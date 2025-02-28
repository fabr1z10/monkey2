#pragma once

#include "../node.h"
#include "../camera.h"
#include "../mouselistener.h"
#include "../components/hotspot.h"
#include "../scheduler.h"
#include <map>


namespace adventure {
    class WalkArea;

    class MouseController : public Node, public MouseListener {
    public:
        MouseController(int camId, WalkArea*,Node*, Scheduler* scheduler, float speed);

        void start() override;

        void cursorPosCallback(GLFWwindow *, double, double) override;

        void mouseButtonCallback(GLFWwindow *, int, int, int) override;

        void setCursor(Node*, const std::vector<std::string>& seq);

        void add(HotSpot*);

        void remove(HotSpot*);

		void setOnEnter(pybind11::function f);

		void setOnLeave(pybind11::function f);

		void setOnClick(pybind11::function f);

		void setOnRightClick(pybind11::function f);
    private:
        bool screenCoordsToWorldCoords(glm::vec2 screenCoords, glm::vec2& worldCoords) const;
        WalkArea *_walkarea;
        Node* _player;
        Node* _cursor;
        Scheduler* _scheduler;
        int _camId;
        OrthoCamera *_cam;
        glm::vec4 _camViewport;
        float _speed;
        std::vector<std::string> _cursorSequence;
        int _cursorType;
        std::map<int, std::unordered_set<HotSpot*>> _hotSpots;
		HotSpot* _previous;
		pybind11::function _onEnter;
		pybind11::function _onLeave;
		pybind11::function _onClick;
		pybind11::function _onRightClick;
    };


}

