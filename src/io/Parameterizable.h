//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_PARAMETERIZABLE_H
#define SRCCMAKE_PARAMETERIZABLE_H


#include <string>
#include <vector>

#include "PropertiesCollection.h"
#include "PropertiesObject.h"


namespace cgf {
    class Parameterizable {

    public:
        Parameterizable (std::string label) {
            m_propertiesCollection = new PropertiesCollection(label);
        }
        ~Parameterizable () {
            delete m_propertiesCollection;
        }

        PropertiesCollection *getPropertiesCollection() {
            return m_propertiesCollection;
        }
    protected:
        void addProperty(PropertiesObject propertiesObject) {
            m_propertiesCollection->addPropertiesObject(propertiesObject);
        }

    private:
        PropertiesCollection *m_propertiesCollection;
    };
}

#endif //SRCCMAKE_PARAMETERIZABLE_H
