//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_GUIMANAGER_H
#define SRCCMAKE_GUIMANAGER_H


#include <vector>
#include <imgui.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "imgui_impl_glfw_gl3.h"

#include "PropertiesCollection.h"


namespace cgf {
    class GuiManager {

    public:
        GuiManager();

        ~GuiManager();

        void render();

        void addPropertiesCollection(cgf::PropertiesCollection *p_propertiesCollection);

    private:
        std::vector<cgf::PropertiesCollection *> *mp_propertiesCollections;

    };
}


#endif //SRCCMAKE_GUIMANAGER_H
