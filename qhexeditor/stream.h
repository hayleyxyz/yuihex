//
// Created by yuikonnu on 19/07/2018.
//

#ifndef PROJECT_STREAM_H
#define PROJECT_STREAM_H


#include <cstdint>

class Stream {
public:
    enum Seek {
        Start,
        Current,
        End
    };

    virtual size_t read(uint8_t *buffer, size_t length)= 0;
    virtual size_t length()= 0;
    virtual int64_t seek(int64_t pos, Seek set)= 0;
    virtual int64_t position()= 0;
};

#endif //PROJECT_STREAM_H
