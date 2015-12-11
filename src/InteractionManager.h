//
// Created by Windrian on 09.12.2015.
//

#ifndef SRCCMAKE_INTERACTIONMANAGER_H
#define SRCCMAKE_INTERACTIONMANAGER_H


#include <vector>
#include <iostream>
#include <functional>
#include <imgui.h>
#include <GLFW/glfw3.h>

#include "Interactable.h"


namespace cgf {
    class InteractionManager {

    public:
        InteractionManager();
        ~InteractionManager();

        void init(GLFWwindow* window);
        void addInteractable(Interactable* interactable);

    private:
        std::vector<Interactable*> m_interactables;

        void keyCallback( int key, int scancode, int action, int mods);
        void cursorCallback(double xPos, double yPos);
        void mouseBtnCallback(int button, int action, int mods);
        void mouseWheelCallback(double xOffset, double yOffset);

        void setKeyCallback(GLFWwindow* window, std::function<void (int, int, int, int)> func);
        void setMouseButtonCallback(GLFWwindow* window, std::function<void (int, int, int)> func);
        void setCursorPosCallback(GLFWwindow* window, std::function<void (double, double)> func);
        void setScrollCallback(GLFWwindow* window, std::function<void (double, double)> func);
    };
}


#endif //SRCCMAKE_INTERACTIONMANAGER_H
