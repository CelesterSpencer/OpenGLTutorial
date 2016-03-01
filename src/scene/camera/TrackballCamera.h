//
// Created by Windrian on 15.11.2015.
//

#ifndef SRCCMAKE_TRACKBALLCAMERA_H
#define SRCCMAKE_TRACKBALLCAMERA_H

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <gtx/rotate_vector.hpp>
#include <gtc/constants.hpp>
#include "Camera.h"


namespace cgf {
    class TrackballCamera : public Camera {

    public:
        TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight);
        TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight, float radius, glm::vec3 target);
        TrackballCamera(GLFWwindow* window, int windowWidth, int windowHeight, float angle, float near, float far, float radius, glm::vec3 target);

        // inherited from Camera
        virtual void update();

        // inherited from Interactable
        virtual bool onKeyboard(int key) override;
        virtual bool onMouse(int x, int y) override;
        virtual bool onMouseScroll(double xOffset, double yOffset) override;
        virtual bool onMouseButton(int button, int action) override;


    private:
        float m_radius;
        float m_theta;
        float m_phi;
        glm::vec2 m_oldPosition;
        bool m_positionReseted = true;
        bool m_mouseButtonPressed = false;

        void rotate(float dTheta, float dPhi);
        void zoom(float distance);
    };
}


#endif //SRCCMAKE_TRACKBALLCAMERA_H
