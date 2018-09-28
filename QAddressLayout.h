//
// Created by yuikonnu on 27/09/2018.
//

#ifndef YUIHEX_QADDRESSDOCUMENT_H
#define YUIHEX_QADDRESSDOCUMENT_H

#include "QHexMetrics.h"
#include <QObject>
#include <QPainter>

class QAddressLayout : public QObject {
    Q_OBJECT

public:
    explicit QAddressLayout(QObject *parent = nullptr);
    void draw(QPainter *painter, QHexMetrics *hexMetrics);
    quint64 address();
    void setAddress(quint64 address);

protected:
    quint64 _address = 0;
    int _bytesPerLine = 16;
    int documentMargin = 4;
};


#endif //YUIHEX_QADDRESSDOCUMENT_H
