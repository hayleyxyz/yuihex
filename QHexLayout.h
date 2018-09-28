//
// Created by yuikonnu on 27/09/2018.
//

#ifndef YUIHEX_QHEXLAYOUT_H
#define YUIHEX_QHEXLAYOUT_H

#include "QHexMetrics.h"
#include "QHexCursor.h"
#include <QObject>
#include <QPainter>

class QHexLayout : public QObject {
    Q_OBJECT

public:
    explicit QHexLayout(QObject *parent = nullptr);
    void draw(QPainter *painter, QHexMetrics *metrics, QHexCursor *cursor);
    int hitTest(QPointF pos, QHexMetrics *metrics, bool exact);
    quint64 textPositionToAddress(int position, QHexMetrics *metrics);
    int textPositionToRemainder(int position, QHexMetrics *metrics);
};


#endif //YUIHEX_QHEXLAYOUT_H
