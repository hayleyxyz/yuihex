//
// Created by yuikonnu on 24/09/2018.
//

#ifndef YUIHEX_QHEXCURSOR_H
#define YUIHEX_QHEXCURSOR_H


#include <cstdint>

class QHexCursor {
public:
    QHexCursor();

    bool isFocusEnabled = false;
    uint64_t startBaseAddress = 0;
    int startTextPosition = 0;
    uint64_t endBaseAddress = 0;
    int endTextPosition = 0;
    void setStartPosition(uint64_t baseAddress, int textPosition);
    void setEndPosition(uint64_t baseAddress, int textPosition);
    uint64_t startAddress();
    uint64_t endAddress();
    int cursorPosition(uint64_t offsetAddress);
    int startSelectionPosition(uint64_t offsetAddress);
    int endSelectionPosition(uint64_t offsetAddress);
    int startAsciiSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine);
    int endAsciiSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine);
    bool isSelection();
    void collapseSelection();
    void setFocusEnabled(bool enabled);
};


#endif //YUIHEX_QHEXCURSOR_H
