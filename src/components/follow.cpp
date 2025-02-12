#include <iostream>
#include "follow.h"
#include "../node.h"
#include "../game.h"
#include "../pyhelper.h"

Follow::Follow (int camId) : Component(), _camId(camId) {
    m_relativePos = glm::vec3(0.f, 0.f, 5.0f);
    m_up = glm::vec3(0,1,0);
    m_previous = glm::vec3(0.0f);
}

void Follow::start() {
    m_cam = Game::instance().getRoom()->getCamera(_camId);
    auto worldPos = m_node->getWorldPosition();
    glm::vec3 eye = worldPos + m_relativePos;
    auto dir = glm::vec3(0.f, 0.f, -1.f);//(glm::normalize(m_node->getWorldPosition() - eye);
    m_cam->setPosition(eye, dir, m_up);
    m_previous = worldPos;

}


void Follow::update (double dt) {
    auto worldPos = m_node->getWorldPosition();
    if (m_previous - worldPos != glm::vec3(0.0f)) {
        glm::vec3 eye = worldPos + m_relativePos;
        auto dir = glm::vec3(0.f, 0.f, -1.f);//(glm::normalize(m_node->getWorldPosition() - eye);
        m_cam->setPosition(eye, dir, m_up);
        //std::cout << m_cam->getViewMatrix()[3][0] << ", " << m_cam->getViewMatrix()[3][1] <<  m_cam->getViewMatrix()[3][2] << "\n";
        m_previous = worldPos;
    }

}