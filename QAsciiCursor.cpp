//
// Created by yuikonnu on 26/09/2018.
//

#include "QAsciiCursor.h"
#include <QtMath>

QAsciiCursor::QAsciiCursor() = default;

bool QAsciiCursor::isSelection() {
    return startBaseAddress != endBaseAddress || startTextPosition != endTextPosition;
}

void QAsciiCursor::setStartPosition(uint64_t baseAddress, int textPosition) {
    startBaseAddress = baseAddress;
    startTextPosition = textPosition;
    endBaseAddress = baseAddress;
    endTextPosition = textPosition;
}

void QAsciiCursor::setEndPosition(uint64_t baseAddress, int textPosition) {
    endBaseAddress = baseAddress;
    endTextPosition = textPosition;
}

uint64_t QAsciiCursor::startAddress() {
    return startBaseAddress + qFloor(startTextPosition / 3);
}

uint64_t QAsciiCursor::endAddress() {
    return endBaseAddress + (endTextPosition / 3);
}

int QAsciiCursor::cursorPosition(uint64_t offsetAddress) {
    return ((startBaseAddress - offsetAddress) * 3) + startTextPosition;
}

int QAsciiCursor::endSelectionPosition(uint64_t offsetAddress) {
    return ((endBaseAddress - offsetAddress) * 3) + endTextPosition;
}

int QAsciiCursor::startSelectionPosition(uint64_t offsetAddress) {
    return qMax(0, cursorPosition(offsetAddress));
}

void QAsciiCursor::collapseSelection() {
    endBaseAddress = startBaseAddress;
    endTextPosition = startTextPosition;
}

void QAsciiCursor::setFocusEnabled(bool enabled) {
    isFocusEnabled = enabled;
}

int QAsciiCursor::endHexSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine) {
    int end = (endBaseAddress - offsetAddress) + qFloor(static_cast<float>(endTextPosition) * 3);
    end -= (end / numberOfBytesPerLine);
    return end;
}

int QAsciiCursor::startHexSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine) {
    int start = (startBaseAddress - offsetAddress) + qFloor(static_cast<float>(startTextPosition) * 3);
    start -= (start / numberOfBytesPerLine);
    return start;
}
