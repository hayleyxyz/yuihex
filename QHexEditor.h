//
// Created by yuikonnu on 24/09/2018.
//

#ifndef YUIHEX_QHEXEDITOR_H
#define YUIHEX_QHEXEDITOR_H

#include "QHexMetrics.h"
#include "QAddressLayout.h"
#include "QHexLayout.h"
#include "QAsciiLayout.h"
#include "QHexCursor.h"
#include <QAbstractScrollArea>

class QHexEditor : public QAbstractScrollArea {
    Q_OBJECT

public:
    explicit QHexEditor(QWidget *parent = nullptr);
    ~QHexEditor();

    /*
     * Events
     */
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

protected:
    quint64 address = 0;
    QHexMetrics *hexMetrics = nullptr;
    QHexCursor *hexCursor = nullptr;
    QAddressLayout addressLayout;
    QHexLayout hexLayout;
    QAsciiLayout asciiLayout;
};


#endif //YUIHEX_QHEXEDITOR_H
