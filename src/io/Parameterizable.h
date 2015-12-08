//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_PARAMETERIZABLE_H
#define SRCCMAKE_PARAMETERIZABLE_H


#include "PropertiesObject.h"

#include <string>
#include <vector>


class Parameterizable {

public:

    std::string getParameterizableName() {
        return m_parameterizableName;
    }
    std::vector<PropertiesObject> getProperties() {
        return m_propertiesObject;
    }

protected:
    void setParametrizableName(std::string parameterizableName) {
        m_parameterizableName = parameterizableName;
    }
    void addProperitesObject(PropertiesObject propertiesObject) {
        m_propertiesObject = propertiesObject;
    }

private:
    std::string m_parameterizableName;
    std::vector<PropertiesObject> m_propertiesObject;

};

#endif //SRCCMAKE_PARAMETERIZABLE_H
