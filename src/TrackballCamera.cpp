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
    float dTheta = (m_oldPosition.x - x) / 2;
    float dPhi = (m_oldPosition.y - y) / 2;
    rotate(-dTheta, dPhi);
    m_oldPosition.x = x;
    m_oldPosition.y = y;
}

bool cgf::TrackballCamera::onMouseScroll(double xOffset, double yOffset) {
    zoom(yOffset);
}

void cgf::TrackballCamera::update() {

    float theta = glm::radians(m_theta);
    float phi = glm::radians(m_phi);

    m_position.x = m_radius * glm::sin(phi) * glm::sin(theta);
    m_position.y = m_radius * glm::cos(phi);
    m_position.z = m_radius * glm::sin(phi) * glm::cos(theta);

    glm::vec3 look = glm::normalize(m_position - m_target);
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    // this will go wrong when the camera is pointing up
    glm::vec3 right = glm::cross(worldUp, look);

    m_up = glm::cross(look, right);
}



void cgf::TrackballCamera::rotate(float dTheta, float dPhi) {
    m_theta += dTheta;
    m_phi += dPhi;
}

void cgf::TrackballCamera::zoom(float distance) {
    m_radius -= distance;
}