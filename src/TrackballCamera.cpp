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
    m_theta = -90.0f;
    m_phi = 0;
    m_target = target;

    glfwSetCursorPos(window, windowWidth/2.0f, windowHeight/2.0f);

    update();
}



bool cgf::TrackballCamera::onKeyboard(int key) {
    // do nothing
    return true;
}

bool cgf::TrackballCamera::onMouse(int x, int y) {

    // do nothing if button is not pressed
    if (!m_mouseButtonPressed) return false;

    // reset old position when mouse button just has been pressed
    if (m_positionReseted) {
        m_oldPosition.x = x;
        m_oldPosition.y = y;
        m_positionReseted = false;
    }

    float dPhi = (m_oldPosition.x - x) / 2;
    float dTheta = (m_oldPosition.y - y) / 2;
    rotate(-dTheta, dPhi);
    m_oldPosition.x = x;
    m_oldPosition.y = y;

    return true;
}

bool cgf::TrackballCamera::onMouseScroll(double xOffset, double yOffset) {
    zoom(yOffset);
    return true;
}

bool cgf::TrackballCamera::onMouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        m_positionReseted = true;
        m_mouseButtonPressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        m_mouseButtonPressed = false;
    }
    return true;
}

void cgf::TrackballCamera::update() {

    float theta = glm::radians(m_theta);
    float phi = glm::radians(m_phi);

    // limit
    static float e = 0.000001f;
    float lowerBorder = 0 + e;
    float upperBorder = glm::pi<float>() - e;
    theta = glm::max(lowerBorder, theta);
    theta = glm::min(theta, upperBorder);

    m_position.x = m_radius * glm::sin(theta) * glm::cos(phi);
    m_position.y = m_radius * glm::cos(theta);
    m_position.z = m_radius * glm::sin(theta) * glm::sin(phi);

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