//
// Created by Windrian on 15.11.2015.
//

#ifndef SRCCMAKE_TRACKBALLCAMERA_H
#define SRCCMAKE_TRACKBALLCAMERA_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <gtx/rotate_vector.hpp>
#include <glfw/glfw3.h>
#include "Camera.h"


namespace cgf {
    class TrackballCamera : public Camera {

    public:
        TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight);
        TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight, float radius, glm::vec3 target);

        bool onKeyboard(int key);
        bool onMouse(int x, int y);
        void update();

    private:
        float m_radius;
        float m_theta;
        float m_phi;
        glm::vec2 m_oldPosition;

        void rotate(float dTheta, float dPhi);
        void zoom(float distance);
    };
}


#endif //SRCCMAKE_TRACKBALLCAMERA_H