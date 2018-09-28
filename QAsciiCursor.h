//
// Created by yuikonnu on 26/09/2018.
//

#ifndef YUIHEX_QASCIICURSOR_H
#define YUIHEX_QASCIICURSOR_H

#include <cstdint>

class QAsciiCursor {
public:
    QAsciiCursor();

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
    int startHexSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine);
    int endHexSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine);
    bool isSelection();
    void collapseSelection();
    void setFocusEnabled(bool enabled);
};


#endif //YUIHEX_QASCIICURSOR_H
