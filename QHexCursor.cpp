//
// Created by yuikonnu on 24/09/2018.
//

#include "QHexCursor.h"
#include <QtMath>

QHexCursor::QHexCursor() = default;

bool QHexCursor::isSelection() {
    return startBaseAddress != endBaseAddress || startTextPosition != endTextPosition;
}

void QHexCursor::setStartPosition(uint64_t baseAddress, int textPosition) {
    startBaseAddress = baseAddress;
    startTextPosition = textPosition;
    endBaseAddress = baseAddress;
    endTextPosition = textPosition;
}

void QHexCursor::setEndPosition(uint64_t baseAddress, int textPosition) {
    endBaseAddress = baseAddress;
    endTextPosition = textPosition;
}

uint64_t QHexCursor::startAddress() {
    return startBaseAddress + qFloor(startTextPosition / 3);
}

uint64_t QHexCursor::endAddress() {
    return endBaseAddress + (endTextPosition / 3);
}

int QHexCursor::cursorPosition(uint64_t offsetAddress) {
    return ((startBaseAddress - offsetAddress) * 3) + startTextPosition;
}

int QHexCursor::endSelectionPosition(uint64_t offsetAddress) {
    return ((endBaseAddress - offsetAddress) * 3) + endTextPosition;
}

int QHexCursor::startSelectionPosition(uint64_t offsetAddress) {
    return qMax(0, cursorPosition(offsetAddress));
}

void QHexCursor::collapseSelection() {
    endBaseAddress = startBaseAddress;
    endTextPosition = startTextPosition;
}

void QHexCursor::setFocusEnabled(bool enabled) {
    isFocusEnabled = enabled;
}

int QHexCursor::endAsciiSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine) {
    int end = (endBaseAddress - offsetAddress) + qCeil(static_cast<float>(endTextPosition) / 3);
    end += (end / numberOfBytesPerLine);
    return end;
}

int QHexCursor::startAsciiSelectionPosition(uint64_t offsetAddress, int numberOfBytesPerLine) {
    int start = (startBaseAddress - offsetAddress) + qCeil(static_cast<float>(startTextPosition) / 3);
    start += (start / numberOfBytesPerLine);
    return start;
}
