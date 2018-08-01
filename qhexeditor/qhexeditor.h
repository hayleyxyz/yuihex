#ifndef QHEXEDITOR_H
#define QHEXEDITOR_H

#include <QAbstractScrollArea>
#include <QTextCursor>
#include <QTextDocument>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include "file_stream.h"
#include "selection.h"
#include "types.h"
#include "cursor.h"


class QHexEditor : public QAbstractScrollArea
{
    Q_OBJECT

    Q_PROPERTY(bool uppercaseHex READ isUppercaseHex WRITE setUppercaseHex)

public:
    QHexEditor(QWidget *parent = 0);
    uint8_t *buffer = nullptr;
    size_t bufferSize = 0;
    addr_t bufferAddress = 0;
    const size_t bufferMaxSize = 1024*1024*2; // 2mb

    FileStream *stream = nullptr;
    addr_t address = 0;
    bool uppercaseHex = false;
    bool cursorBlink = false;

    bool isUppercaseHex() const;
    void setUppercaseHex(bool value);

    Selection currentSelection;
    Cursor textCursor;

    bool openFile(const QString &file);
    void closeFile();

signals:
    void fileOpened(const QString &file);

protected:
    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    size_t numberOfBytesInDisplay();
    size_t numberOfBytesPerLine();
    size_t numberOfLines();
    int numberOfAddressChars();

    int charWidth();
    int charHeight();
    int charAscent();
    int _charWidth = 0;
    int _charHeight = 0;
    int _charAscent = 0;

    QColor primarySelectionColour();
    QColor secondarySelectionColour();

    void drawHexSelections(QPainter &painter);
    void drawSelections(QPainter &painter);

    QPoint getHexCharCoordForPosition(int x, int y);
    QRect getHexViewBoundingBox();
    QPoint getHexTextPosition(int x, int y);
    QString formatHexByteText(uint8_t byte);
    void drawHexView(QPainter &painter);

    QPoint getAddressTextPosition(int y);
    QString formatAddressText(addr_t address);
    void drawAddressView(QPainter &painter);

    QPoint getAsciiTextPosition(int x, int y);
    QString formatAsciiByteText(uint8_t byte);
    void drawAsciiView(QPainter &painter);
    void drawAsciiSelections(QPainter &painter);
    QRect getAsciiViewBoundingBox();
    QPoint getAsciiCharCoordForPosition(int x, int y);

    void drawCursor(QPainter &painter);

    void fillBuffer();
};

#endif
