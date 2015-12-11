//
// Created by Windrian on 08.12.2015.
//

#ifndef SRCCMAKE_PARAMETERIZABLE_H
#define SRCCMAKE_PARAMETERIZABLE_H


#include <string>
#include <vector>

#include "PropertiesCollection.h"


namespace cgf {
    class Parameterizable {

    public:
        virtual PropertiesCollection* getPropertiesCollection() =0;

    };
}

#endif //SRCCMAKE_PARAMETERIZABLE_H
