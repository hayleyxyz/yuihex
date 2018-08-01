//
// Created by yuikonnu on 20/07/2018.
//

#ifndef PROJECT_SELECTION_H
#define PROJECT_SELECTION_H


#include "types.h"

class Selection {
public:
    enum Window {
        Hex,
        Ascii
    };

    addr_t start, end;
    bool active = false;
    Window window;
};


#endif //PROJECT_SELECTION_H
