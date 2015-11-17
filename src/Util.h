//
// Created by Windrian on 14.11.2015.
//

#ifndef SRCCMAKE_UTIL_H
#define SRCCMAKE_UTIL_H

#include <iostream>
#include <glm.hpp>

class Util {

public:
    static void printMat(glm::mat4x4& m, std::string name) {
        std::cout << name << "=" << std::endl;
        for(int row = 0; row < 4; row++) {
            std::cout << "( ";
            for(int col = 0; col < 4; col++) {
                std::cout << m[col][row] << " ";
            }
            std::cout << ")" << std::endl;
        }
    }

};

#endif //SRCCMAKE_UTIL_H
