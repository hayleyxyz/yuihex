//
// Created by Oscar Berry on 19/07/2018.
//

#include "file_stream.h"

bool FileStream::open(std::string filePath, FileStream::Mode mode) {
    const char *chMode = this->resolveMode(mode);
    if(chMode == nullptr) {
        return false;
    }

    auto file = std::fopen(filePath.c_str(), chMode);
    if(file == nullptr) {
        return false;
    }

    this->handle = file;

    return true;
}

const char *FileStream::resolveMode(FileStream::Mode mode) {
    switch(mode) {
        case FileStream::Mode::OpenRead:
            return "rb";
        case FileStream::Mode::OpenReadWrite:
            return "rb+";
        default:
            return nullptr;
    }
}

size_t FileStream::read(uint8_t *buffer, size_t length) {
    return std::fread(buffer, length, 1, this->handle);
}

void FileStream::close() {
    std::fclose(this->handle);
}

size_t FileStream::length() {
    auto pos = std::ftell(this->handle);

    std::fseek(this->handle, 0, SEEK_END);
    auto length = std::ftell(this->handle);
    std::fseek(this->handle, pos, SEEK_SET);

    return length;
}

int64_t FileStream::seek(int64_t pos, Stream::Seek seekMode) {
    auto mode = this->resolveSeekMode(seekMode);
    return ::fseek(this->handle, pos, mode);
}

int FileStream::resolveSeekMode(Stream::Seek seek) {
    switch(seek) {
        case Seek::Current:
            return SEEK_CUR;
        case Seek::Start:
            return SEEK_SET;
        case Seek::End:
            return SEEK_END;
        default:
            return 0;
    }
}

int64_t FileStream::position() {
    return ::ftell(this->handle);
}

FileStream::~FileStream() {

}
