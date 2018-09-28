//
// Created by yuikonnu on 28/09/2018.
//

#include "QHexCursor.h"
#include <QtMath>

QHexCursor::QHexCursor(QObject *parent) : QObject(parent) {

}

bool QHexCursor::isSelection() {
    return _startAddress != _endAddress || _startHexPartial != _endHexPartial || _forceSelection;
}

bool QHexCursor::isReverseSelection() {
    return (_startAddress >= _endAddress) && (_startHexPartial > _endHexPartial);
}

void QHexCursor::setForceSelection(bool forceSelection) {
    _forceSelection = forceSelection;
}

bool QHexCursor::isSelecting() {
    return _isSelecting;
}

void QHexCursor::setSelecting(bool selecting) {
    _isSelecting = selecting;
}

quint64 QHexCursor::startAddress() {
    return _startAddress;
}

quint64 QHexCursor::endAddress() {
    return _endAddress;
}

void QHexCursor::setStartAddress(quint64 address) {
    _startAddress = address;
}

void QHexCursor::setEndAddress(quint64 address) {
    _endAddress = address;
}

void QHexCursor::setStartHexCharIndex(int index) {
    _startHexPartial = index;
}

void QHexCursor::setEndHexCharIndex(int index) {
    _endHexPartial = index;
}

int QHexCursor::startHexCharIndex() {
    return _startHexPartial;
}

int QHexCursor::endHexCharIndex() {
    return _endHexPartial;
}

int QHexCursor::startHexTextPosition(int bytesPerLine) {
    return addressToHexTextPosition(_startAddress, bytesPerLine) + startHexCharIndex();
}

int QHexCursor::endHexTextPosition(int bytesPerLine) {
    return addressToHexTextPosition(_endAddress, bytesPerLine) + endHexCharIndex();
}

int QHexCursor::addressToHexTextPosition(quint64 address, int bytesPerLine) {
    auto result = address * 3;
    result -= qFloor(address / bytesPerLine);
    return result;
}
