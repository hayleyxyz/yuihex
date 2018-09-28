//
// Created by yuikonnu on 27/09/2018.
//

#include "QAddressLayout.h"
#include <QtMath>

QAddressLayout::QAddressLayout(QObject *parent) : QObject(parent) {

}

void QAddressLayout::draw(QPainter *painter, QHexMetrics *metrics) {
    for(auto y = 0; y < metrics->lineCount(); y++) {
        auto pos = metrics->getAddressLinePosition(y);
        QString str("%1");
        auto addr = static_cast<qulonglong>(address() + (metrics->bytesPerLine() * y));
        painter->setPen(QColor(125, 125, 125));
        painter->drawText(pos.x(), qCeil(pos.y()), str.arg(addr, 8, 16, QLatin1Char('0')));

    }
}

quint64 QAddressLayout::address() {
    return _address;
}

void QAddressLayout::setAddress(quint64 address) {
    _address = address;
}