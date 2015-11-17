//
// Created by Windrian on 15.11.2015.
//

#include "TrackballCamera.h"



cgf::TrackballCamera::TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m_radius = 1;
    m_theta = 0;
    m_phi = 0;
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);

    glfwSetCursorPos(window, windowWidth/2.0f, windowHeight/2.0f);
    m_oldPosition.x = windowWidth/2.0f;
    m_oldPosition.y = windowHeight/2.0f;

    update();
}

cgf::TrackballCamera::TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight, float radius, glm::vec3 target) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m_radius = radius;
    m_theta = 0;
    m_phi = 0;
    m_target = target;

    glfwSetCursorPos(window, windowWidth/2.0f, windowHeight/2.0f);

    update();
}



bool cgf::TrackballCamera::onKeyboard(int key) {
    // do nothing
}

bool cgf::TrackballCamera::onMouse(int x, int y) {
    float dPhi = 2 * M_PI * (m_oldPosition.y - y) / m_windowHeight;
    float dTheta = M_PI * (m_oldPosition.x - x) / m_windowWidth;
    rotate(-dTheta, dPhi);
    m_oldPosition.x = x;
    m_oldPosition.y = y;
}

void cgf::TrackballCamera::update() {

    m_position.x = m_radius * sinf(m_theta) * cosf(m_phi);
    m_position.y = m_radius * cosf(m_theta);
    m_position.z = m_radius * sinf(m_theta) * sinf(m_phi);

    glm::vec3 look = glm::normalize(m_position - m_target);
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    // this will go wrong when the camera is pointing up
    glm::vec3 right = glm::cross(worldUp, look);

    m_up = glm::cross(look, right);
}



void cgf::TrackballCamera::rotate(float dTheta, float dPhi) {
    m_theta += dTheta;
    m_phi += dPhi;
    std::cout << "rotate to " << m_theta << ", " << m_phi << std::endl;
}

void cgf::TrackballCamera::zoom(float distance) {
    m_radius -= distance;
}