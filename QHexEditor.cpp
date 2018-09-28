//
// Created by yuikonnu on 24/09/2018.
//

#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include "QHexEditor.h"
#include "easylogging++.h"

QHexEditor::QHexEditor(QWidget *parent) : QAbstractScrollArea(parent) {
    hexMetrics = new QHexMetrics(this);
    hexCursor = new QHexCursor(this);
}

QHexEditor::~QHexEditor() {

}

void QHexEditor::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());

    addressLayout.draw(&painter, hexMetrics);
    hexLayout.draw(&painter, hexMetrics, hexCursor);
    asciiLayout.draw(&painter, hexMetrics, hexCursor);
}

void QHexEditor::mousePressEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::MouseButton::LeftButton)) {
        auto hexPosition = hexLayout.hitTest(event->pos(), hexMetrics, true);
        if (hexPosition >= 0) {
            auto addr = hexLayout.textPositionToAddress(hexPosition, hexMetrics);
            auto remainder = hexLayout.textPositionToRemainder(hexPosition, hexMetrics);

            LOG(INFO) << "remainder: " << remainder;

            hexCursor->setStartAddress(addr);
            hexCursor->setStartHexCharIndex(remainder);
            hexCursor->setEndAddress(addr);
            hexCursor->setEndHexCharIndex(remainder);
            hexCursor->setSelecting(true);
            hexCursor->setForceSelection(false);

            viewport()->update();
        }
    }

    QAbstractScrollArea::mousePressEvent(event);
}

void QHexEditor::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::MouseButton::LeftButton) && hexCursor->isSelecting()) {
        auto hexPosition = hexLayout.hitTest(event->pos(), hexMetrics, false);
        if(hexPosition >= 0) {
            auto addr = hexLayout.textPositionToAddress(hexPosition, hexMetrics);
            auto remainder = hexLayout.textPositionToRemainder(hexPosition, hexMetrics);

            hexCursor->setEndAddress(addr);
            hexCursor->setEndHexCharIndex(remainder);
            hexCursor->setForceSelection(true);

            viewport()->update();
        }
    }
    else {
        hexCursor->setSelecting(false);
    }


    QAbstractScrollArea::mouseMoveEvent(event);
}

void QHexEditor::mouseReleaseEvent(QMouseEvent *event) {
    hexCursor->setSelecting(false);

    QAbstractScrollArea::mouseReleaseEvent(event);
}
