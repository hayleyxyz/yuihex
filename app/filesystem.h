//
// Created by yuikonnu on 01/08/2018.
//

#ifndef PROJECT_FILESYSTEM_H
#define PROJECT_FILESYSTEM_H

#include <string>
#include <sys/stat.h>

namespace filesystem {

static inline bool is_dir(std::string path) {
    struct stat info{};
    ::stat(path.c_str(), &info);

    return S_ISDIR(info.st_mode);
}

static inline bool is_read_only(std::string path) {
    return false;
}

}

#endif //PROJECT_FILESYSTEM_H
