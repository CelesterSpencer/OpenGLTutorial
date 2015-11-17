//
// Created by Windrian on 14.11.2015.
//

#ifndef SRCCMAKE_CAMERA_H
#define SRCCMAKE_CAMERA_H

#include <glm.hpp>


namespace cgf {
    class Camera {
    public:
        virtual bool onKeyboard(int key) =0;
        virtual bool onMouse(int x, int y) =0;
        virtual void update() =0;

        glm::vec3 getPosition() { return m_position; };
        glm::vec3 getTarget() { return m_target; };
        glm::vec3 getUp() { return m_up; };

        void setWindowSize(int windowWidth, int windowHeight) {
            m_windowWidth = windowWidth;
            m_windowHeight = windowHeight;
        }

    protected:
        glm::vec3 m_position;
        glm::vec3 m_target;
        glm::vec3 m_up;

        int m_windowWidth;
        int m_windowHeight;
    };
}


#endif //SRCCMAKE_CAMERA_H
