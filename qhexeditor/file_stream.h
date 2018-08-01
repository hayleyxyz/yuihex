//
// Created by Oscar Berry on 19/07/2018.
//

#ifndef PROJECT_FILE_STREAM_H
#define PROJECT_FILE_STREAM_H

#include <string>
#include <cstdio>
#include "stream.h"

class FileStream : public Stream {
public:
    ~FileStream();

    enum Mode {
        OpenRead = 0b1,
        OpenReadWrite = 0b10,
    };


    bool open(std::string file, Mode mode = FileStream::Mode::OpenReadWrite);
    size_t read(uint8_t *buffer, size_t length) override;
    size_t length() override;
    int64_t seek(int64_t pos, Stream::Seek set) override;
    int64_t position() override;

    void close();

protected:
    FILE * handle = nullptr;
    const char *resolveMode(Mode mode);

    int resolveSeekMode(Seek seek);
};

#endif //PROJECT_FILE_STREAM_H
