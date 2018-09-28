//
// Created by yuikonnu on 27/09/2018.
//

#ifndef YUIHEX_QASCIILAYOUT_H
#define YUIHEX_QASCIILAYOUT_H

#include "QHexMetrics.h"
#include "QHexCursor.h"
#include <QObject>
#include <QPainter>

class QAsciiLayout : public QObject {
    Q_OBJECT

public:
    explicit QAsciiLayout(QObject *parent = nullptr);
    void draw(QPainter *painter, QHexMetrics *metrics, QHexCursor *cursor);
    int hitTest(QPointF pos, QHexMetrics *metrics, bool exact);
};


#endif //YUIHEX_QASCIILAYOUT_H
