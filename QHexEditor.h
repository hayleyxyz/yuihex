//
// Created by yuikonnu on 24/09/2018.
//

#ifndef YUIHEX_QHEXEDITOR_H
#define YUIHEX_QHEXEDITOR_H

#include "QHexCursor.h"
#include "QAsciiCursor.h"
#include <QAbstractScrollArea>
#include <QTextDocument>
#include <QFile>

class QHexEditor : public QAbstractScrollArea {
    Q_OBJECT



public:
    explicit QHexEditor(QWidget *parent = nullptr);
    bool openFile(const QString &file);
    void closeFile();
    bool isFileOpen();

protected:
    void init();
    bool event(QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void paintAddress(QPainter &painter);
    void paintHex(QPainter &painter);
    void paintAscii(QPainter &painter);
    size_t numberOfLines();
    const size_t numberOfBytesPerLine();
    const size_t numberOfAddressChars();
    bool addressIsInView(uint64_t address);
    QPointF getAddressDocumentPosition();
    QPointF getHexDocumentPosition();
    QPointF getAsciiDocumentPosition();
    void updateContents();
    void updateAddressText();
    void updateHexText();
    void updateAsciiText();
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void fillBuffer();
    size_t numberOfBytesInDisplay();


    QFile *file = nullptr;
    QByteArray buffer;
    uint64_t bufferAddress = 0;
    const size_t bufferMaxSize = 1024*1024*2; // 2mb

    uint64_t address = 0;
    QTextDocument *addressDocument = nullptr;
    QTextDocument *hexDocument = nullptr;
    QTextDocument *asciiDocument = nullptr;
    QHexCursor hexCursor;
    QAsciiCursor asciiCursor;

    int cursorBlinkTimer = 0;
    bool cursorBlinkStatus = true;

};


#endif //YUIHEX_QHEXEDITOR_H
