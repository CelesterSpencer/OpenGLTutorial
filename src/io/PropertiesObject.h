//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_PROPERTIESOBJECT_H
#define SRCCMAKE_PROPERTIESOBJECT_H


#include <string>


namespace cgf {
    class PropertiesObject {

    public:
        std::string m_label;
        void *mp_value;
        enum PropertyType {
            INT,
            FLOAT,
            VEC3
        };
        PropertyType m_type;

        PropertiesObject(std::string label, void *value, PropertyType type) {
            m_label = label;
            mp_value = value;
            m_type = type;
        }

    };
}

#endif //SRCCMAKE_PROPERTIESOBJECT_H
