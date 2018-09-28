//
// Created by yuikonnu on 27/09/2018.
//

#include "QHexMetrics.h"
#include "QHexEditor.h"
#include <QtMath>

QHexMetrics::QHexMetrics(QHexEditor *parent) : QObject(parent) {

}

QFontMetricsF QHexMetrics::fontMetrics() {
    auto hex = dynamic_cast<QHexEditor*>(this->parent());
    return QFontMetricsF(hex->font());
}

int QHexMetrics::lineCount() {
    auto hex = dynamic_cast<QHexEditor*>(this->parent());
    auto height = hex->viewport()->height() - (textDocumentMargin() * 2);
    return qFloor(height /  hex->fontMetrics().lineSpacing());
}

int QHexMetrics::textDocumentMargin() {
    return 4;
}

int QHexMetrics::bytesPerLine() {
    return 16;
}

int QHexMetrics::numberOfAddressDigits() {
    return 8;
}

int QHexMetrics::hexCharactersPerLine() {
    return (bytesPerLine() * 3) - 1;
}


QPointF QHexMetrics::addressLayoutPosition() {
    return QPointF(textDocumentMargin(), textDocumentMargin());
}

QSizeF QHexMetrics::addressLayoutSize() {
    return QSizeF(
            fontMetrics().maxWidth() * numberOfAddressDigits(),
            fontMetrics().lineSpacing() * lineCount()
    );
}

QRectF QHexMetrics::addressLayoutBoundingBox() {
    return QRectF(addressLayoutPosition(), addressLayoutSize());
}

QPointF QHexMetrics::getAddressLinePosition(int line) {
    auto fontMetrics = this->fontMetrics();
    auto point = addressLayoutPosition() +
            QPointF(0, fontMetrics.ascent()) +
            QPointF(0, fontMetrics.lineSpacing() * line);

    return point;
}

QPointF QHexMetrics::hexLayoutPosition() {
    return addressLayoutBoundingBox().topRight() +
            QPointF(textDocumentMargin(), 0);
}

QSizeF QHexMetrics::hexLayoutSize() {
    return QSizeF(
            fontMetrics().maxWidth() * hexCharactersPerLine(),
            fontMetrics().lineSpacing() * lineCount()
    );
}

QRectF QHexMetrics::hexLayoutBoundingBox() {
    return QRectF(hexLayoutPosition(), hexLayoutSize());
}

QPointF QHexMetrics::getHexBytePosition(int x, int y) {
    auto pos = hexLayoutPosition() +
               QPointF(0, fontMetrics().ascent()) +
               QPointF(fontMetrics().maxWidth() * 3 * x, fontMetrics().lineSpacing() * y);

    return pos;
}

QPointF QHexMetrics::getHexCharacterPosition(int x, int y) {
    auto pos = hexLayoutPosition() +
               QPointF(0, fontMetrics().ascent()) +
               QPointF(fontMetrics().maxWidth() * x, fontMetrics().lineSpacing() * y);

    return pos;
}

QRectF QHexMetrics::getHexCharacterBoundingBox(int textPosition) {
    return getHexCharacterBoundingBox(
            textPosition % hexCharactersPerLine(),
            qFloor(textPosition / hexCharactersPerLine())
    );
}

QRectF QHexMetrics::getHexCharacterBoundingBox(int x, int y) {
    return QRectF(
            getHexCharacterPosition(x, y) - QPointF(0, fontMetrics().ascent()),
            QSizeF(fontMetrics().maxWidth(), fontMetrics().lineSpacing())
    );
}

QPointF QHexMetrics::asciiLayoutPosition() {
    return hexLayoutBoundingBox().topRight() +
            QPointF(textDocumentMargin(), 0);
}

QSizeF QHexMetrics::asciiLayoutSize() {
    return QSizeF(
            fontMetrics().maxWidth() * bytesPerLine(),
            fontMetrics().lineSpacing() * lineCount()
    );
}

QRectF QHexMetrics::asciiLayoutBoundingBox() {
    return QRectF(asciiLayoutPosition(), asciiLayoutSize());
}

QPointF QHexMetrics::getAsciiBytePosition(int x, int y) {
    auto pos = asciiLayoutPosition() +
               QPointF(0, fontMetrics().ascent()) +
               QPointF(fontMetrics().maxWidth() * x, fontMetrics().lineSpacing() * y);

    return pos;
}
