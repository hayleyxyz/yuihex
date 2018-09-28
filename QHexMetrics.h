//
// Created by yuikonnu on 27/09/2018.
//

#ifndef YUIHEX_QHEXMETRICS_H
#define YUIHEX_QHEXMETRICS_H

class QHexEditor;

#include <QObject>
#include <QFontMetricsF>

class QHexMetrics : public QObject {
    Q_OBJECT

public:
    explicit QHexMetrics(QHexEditor *parent = nullptr);
    QFontMetricsF fontMetrics();
    int bytesPerLine();
    int lineCount();
    int textDocumentMargin();
    int numberOfAddressDigits();
    int hexCharactersPerLine();
    QPointF addressLayoutPosition();
    QSizeF addressLayoutSize();
    QRectF addressLayoutBoundingBox();
    QPointF getAddressLinePosition(int line);
    QPointF getHexBytePosition(int x, int y);
    QPointF getHexCharacterPosition(int x, int y);
    QRectF getHexCharacterBoundingBox(int textPosition);
    QRectF getHexCharacterBoundingBox(int x, int y);
    QPointF hexLayoutPosition();
    QSizeF hexLayoutSize();
    QRectF hexLayoutBoundingBox();
    QPointF asciiLayoutPosition();
    QSizeF asciiLayoutSize();
    QRectF asciiLayoutBoundingBox();
    QPointF getAsciiBytePosition(int x, int y);

};


#endif //YUIHEX_QHEXMETRICS_H
