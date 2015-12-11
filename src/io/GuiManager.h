//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_GUIMANAGER_H
#define SRCCMAKE_GUIMANAGER_H


#include <vector>
#include <imgui.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw_gl3.h"

#include "PropertiesCollection.h"
#include "Interactable.h"


namespace cgf {
    class GuiManager : public Interactable {

    public:
        GuiManager(GLFWwindow* window);
        ~GuiManager();

        void render();
        void addPropertiesCollection(cgf::PropertiesCollection *p_propertiesCollection);
        void removePropertiesCollection(cgf::PropertiesCollection *p_propertiesCollection);

        bool onKeyboard(int key) override;
        bool onMouse(int x, int y) override;
        bool onMouseScroll(double xOffset, double yOffset) override;
        bool onMouseButton(int button, int action) override;

    private:
        std::vector<cgf::PropertiesCollection *> *mp_propertiesCollections;

    };
}


#endif //SRCCMAKE_GUIMANAGER_H
