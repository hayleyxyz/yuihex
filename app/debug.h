//
// Created by yuikonnu on 01/08/2018.
//

#ifndef PROJECT_DEBUG_H
#define PROJECT_DEBUG_H

#if defined(__GNUC__)
#define     always_inline __attribute__((always_inline))
#else
#define     always_inline
#endif

always_inline inline void __debugbreak() {
    __asm volatile("int $3");
}

#endif //PROJECT_DEBUG_H
