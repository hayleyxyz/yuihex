//
// Created by yuikonnu on 27/09/2018.
//

#include "QHexLayout.h"
#include "QHexCursor.h"
#include "easylogging++.h"

#include <QtMath>

QHexLayout::QHexLayout(QObject *parent) : QObject(parent) {

}

void QHexLayout::draw(QPainter *painter, QHexMetrics *metrics, QHexCursor *cursor) {
    // Draw cursor
    auto cursorPosition = cursor->startHexTextPosition(metrics->bytesPerLine());
    auto cursorRect = metrics->getHexCharacterBoundingBox(cursorPosition);
    cursorRect.setWidth(1);
    painter->fillRect(cursorRect, QColor(0, 0, 0));

    if(cursor->isSelection()) {
        auto start = cursor->startHexTextPosition(metrics->bytesPerLine());
        auto end = cursor->endHexTextPosition(metrics->bytesPerLine());
        auto from = qMin(start, end);
        auto to = qMax(start, end);

        if(start > end) {
            to--;
        }

        for(auto pos = from; pos <= to; pos++) {
            auto rect = metrics->getHexCharacterBoundingBox(pos);
            painter->fillRect(rect, QColor(0, 255, 0));
        }
    }

    for(auto y = 0; y < metrics->lineCount(); y++) {
        for(auto x = 0; x < metrics->bytesPerLine(); x++) {
            auto pos = metrics->getHexBytePosition(x, y);
            painter->drawText(pos, QStringLiteral("00"));
        }
    }
}

int QHexLayout::hitTest(QPointF pos, QHexMetrics *metrics, bool exact) {
    if(exact && !metrics->hexLayoutBoundingBox().contains(pos)) {
        return -1;
    }

    // Get the position relative to the layout
    auto relative = pos - metrics->hexLayoutPosition();
    auto size = metrics->hexLayoutSize();
    auto point = QPointF(
            qMin(qMax(relative.x(), 0.0), size.width() - metrics->fontMetrics().maxWidth()),
            qMin(qMax(relative.y(), 0.0), size.height() - metrics->fontMetrics().lineSpacing())
    );

    auto x = qFloor(point.x() / metrics->fontMetrics().maxWidth());
    auto y = qFloor(point.y() / metrics->fontMetrics().lineSpacing());

    return (y * metrics->hexCharactersPerLine()) + x;
}

quint64 QHexLayout::textPositionToAddress(int position, QHexMetrics *metrics) {
    return (position + qFloor(position / metrics->hexCharactersPerLine())) / 3;
}

int QHexLayout::textPositionToRemainder(int position, QHexMetrics *metrics) {
    return (position + qFloor(position / metrics->hexCharactersPerLine())) % 3;
}
