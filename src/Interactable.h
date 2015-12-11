//
// Created by Windrian on 09.12.2015.
//

#ifndef SRCCMAKE_INTERACTABLE_H
#define SRCCMAKE_INTERACTABLE_H

namespace cgf {
    class Interactable {

    public:
        void setConsumeBehavior(bool doesConsumeInput) {
            m_doesConsumeInput = doesConsumeInput;
        }

        virtual bool onKeyboard(int key) =0;
        virtual bool onMouse(int x, int y) =0;
        virtual bool onMouseScroll(double xOffset, double yOffset) =0;
        virtual bool onMouseButton(int button, int action) =0;

    protected:
        bool m_doesConsumeInput = true;

    };
}
#endif //SRCCMAKE_INTERACTABLE_H
