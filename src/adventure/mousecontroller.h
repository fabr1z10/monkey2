#pragma once

#include "../node.h"
#include "../camera.h"
#include "../mouselistener.h"

#include "../scheduler.h"


namespace adventure {
    class WalkArea;

    class MouseController : public Node, public MouseListener {
    public:
        MouseController(int camId, WalkArea*,Node*, Scheduler* scheduler, float speed);

        void start() override;

        void cursorPosCallback(GLFWwindow *, double, double) override;

        void mouseButtonCallback(GLFWwindow *, int, int, int) override;

    private:
        bool screenCoordsToWorldCoords(glm::vec2 screenCoords, glm::vec2& worldCoords) const;
        WalkArea *_walkarea;
        Node* _player;
        Scheduler* _scheduler;
        int _camId;
        OrthoCamera *_cam;
        glm::vec4 _camViewport;
        float _speed;
    };
}