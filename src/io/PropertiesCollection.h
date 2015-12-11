//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_PROPERTIESCOLLECTION_H
#define SRCCMAKE_PROPERTIESCOLLECTION_H


#include <string>
#include <vector>

#include "PropertiesObject.h"


namespace cgf {
    class PropertiesCollection {

    public:
        PropertiesCollection(std::string name) {
            m_name = name;
        }

        std::string getName() {
            return m_name;
        }

        void addPropertiesObject(PropertiesObject p_propertiesObject) {
            mp_propertyObjects.push_back(p_propertiesObject);
        }

        std::vector<PropertiesObject> getPropertyObjects() {
            return mp_propertyObjects;
        }

    private:
        std::string m_name;
        std::vector<PropertiesObject> mp_propertyObjects;

    };
}

#endif //SRCCMAKE_PROPERTIESCOLLECTION_H
