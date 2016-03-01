//
// Created by Windrian on 14.11.2015.
//

#ifndef SRCCMAKE_CAMERA_H
#define SRCCMAKE_CAMERA_H

#include <glm.hpp>
#include <glfw/glfw3.h>

#include "Interactable.h"


namespace cgf {
    class Camera : public Interactable {
    public:

        // getter and setter
        glm::vec3 getPosition() { return m_position; };
        glm::vec3 getTarget() { return m_target; };
        glm::vec3 getUp() { return m_up; };
        void setProjection(int windowWidth, int windowHeight, float angle, float zNear, float zFar) {
            m_windowWidth = windowWidth;
            m_windowHeight = windowHeight;
            m_angle = angle;
            m_near = zNear;
            m_far = zFar;
        }

        int getImageplaneWidth() { return m_windowWidth; };
        int getImageplaneHeight() { return m_windowHeight; };
        float getCameraangle() { return m_angle; };
        float getNear() { return m_near; };
        float getFar() { return m_far; };

        virtual void update() =0;

        // inherited from Interactable
        virtual bool onKeyboard(int key) =0;
        virtual bool onMouse(int x, int y) =0;
        virtual bool onMouseScroll(double xOffset, double yOffset) =0;
        virtual bool onMouseButton(int button, int action) =0;

    protected:
        glm::vec3 m_position;
        glm::vec3 m_target;
        glm::vec3 m_up;

        int m_windowWidth;
        int m_windowHeight;
        float m_angle;
        float m_near;
        float m_far;
    };
}


#endif //SRCCMAKE_CAMERA_H
