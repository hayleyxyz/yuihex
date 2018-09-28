//
// Created by yuikonnu on 27/09/2018.
//

#include "QAsciiLayout.h"
#include "QHexCursor.h"

#include <QtMath>

QAsciiLayout::QAsciiLayout(QObject *parent) : QObject(parent) {

}

void QAsciiLayout::draw(QPainter *painter, QHexMetrics *metrics, QHexCursor *pCursor) {
    for(auto y = 0; y < metrics->lineCount(); y++) {
        for(auto x = 0; x < metrics->bytesPerLine(); x++) {
            auto pos = metrics->getAsciiBytePosition(x, y);
            painter->drawText(pos, QStringLiteral("."));
        }
    }
}

int QAsciiLayout::hitTest(QPointF pos, QHexMetrics *metrics, bool exact) {
    if(metrics->asciiLayoutBoundingBox().contains(pos)) {
        // Get the position relative to the layout
        auto relative = pos - metrics->asciiLayoutPosition();
        auto x = qFloor(relative.x() / metrics->fontMetrics().maxWidth());
        auto y = qFloor(relative.y() / metrics->fontMetrics().lineSpacing());

        return (y * metrics->bytesPerLine()) + x;
    }

    return -1;
}
