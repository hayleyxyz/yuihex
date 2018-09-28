//
// Created by yuikonnu on 24/09/2018.
//

#include "QHexEditor.h"
#include "easylogging++.h"
#include <QPainter>
#include <QEvent>
#include <QtMath>
#include <sstream>
#include <iomanip>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QMimeData>
#include <QFile>
#include <QGuiApplication>
#include <QStyleHints>


QHexEditor::QHexEditor(QWidget *parent) : QAbstractScrollArea(parent) {
    init();
}

void QHexEditor::init() {
    addressDocument = new QTextDocument();
    hexDocument = new QTextDocument();
    asciiDocument = new QTextDocument();

    int flashTime = QGuiApplication::styleHints()->cursorFlashTime();
    if (flashTime >= 2) {
        cursorBlinkTimer = startTimer(flashTime / 2);
    }

    cursorBlinkStatus = true;
}

bool QHexEditor::event(QEvent *event) {
    LOG(INFO) << "event.type: " << event->type();

    switch(event->type()) {

        case QEvent::Type::Resize:
        case QEvent::Type::Scroll:
        case QEvent::Type::WindowActivate:
        case QEvent::Type::Wheel:
            this->updateContents();
            viewport()->update();
            break;

        case QEvent::Type::LayoutRequest:
        case QEvent::Type::Paint:
            this->updateContents();
            break;
    }

    return QAbstractScrollArea::event(event);
}

void QHexEditor::timerEvent(QTimerEvent *event) {
    if(event->timerId() == cursorBlinkTimer) {
        cursorBlinkStatus = !cursorBlinkStatus;
        viewport()->update();
    }

    QObject::timerEvent(event);
}


void QHexEditor::mousePressEvent(QMouseEvent *event) {
    auto hexBB = hexDocument->documentLayout()->frameBoundingRect(hexDocument->rootFrame());
    hexBB.moveTo(getHexDocumentPosition());

    auto asciiBB = asciiDocument->documentLayout()->frameBoundingRect(asciiDocument->rootFrame());
    asciiBB.moveTo(getAsciiDocumentPosition());

    if((event->buttons() & Qt::MouseButton::LeftButton) > 0) {
        // Check if mouse click was on hex text
        if(hexBB.contains(event->pos())) {
            auto offsetPos = event->pos() - getHexDocumentPosition();
            auto pos = hexDocument->documentLayout()->hitTest(offsetPos, Qt::HitTestAccuracy::ExactHit);

            if (pos != -1) {
                if ((event->modifiers() & Qt::KeyboardModifier::ShiftModifier) > 0) {
                    hexCursor.setEndPosition(address, pos);
                }
                else {
                    LOG(INFO) << "pos" << (pos % 3);
                    hexCursor.setStartPosition(address, pos);
                    hexCursor.setFocusEnabled(true);
                }

                asciiCursor.collapseSelection();
                asciiCursor.setFocusEnabled(false);
            }
        }
        else if(asciiBB.contains(event->pos())) { // Next check if click was on ascii
            auto offsetPos = event->pos() - getAsciiDocumentPosition();
            auto pos = asciiDocument->documentLayout()->hitTest(offsetPos, Qt::HitTestAccuracy::ExactHit);

            if (pos != -1) {
                hexCursor.collapseSelection();
                hexCursor.setFocusEnabled(false);


                if ((event->modifiers() & Qt::KeyboardModifier::ShiftModifier) > 0) {
                    asciiCursor.setEndPosition(address, pos);
                }
                else {
                    asciiCursor.setStartPosition(address, pos);
                    asciiCursor.setFocusEnabled(true);
                }
            }
        }

        viewport()->update();
    }

    QAbstractScrollArea::mousePressEvent(event);
}

void QHexEditor::mouseReleaseEvent(QMouseEvent *event) {
    QAbstractScrollArea::mouseReleaseEvent(event);
}

void QHexEditor::mouseDoubleClickEvent(QMouseEvent *event) {
    QAbstractScrollArea::mouseDoubleClickEvent(event);
}

void QHexEditor::mouseMoveEvent(QMouseEvent *event) {
    if((event->buttons() & Qt::MouseButton::LeftButton) > 0) {
        if(hexCursor.isFocusEnabled) {
            auto offsetPos = event->pos() - getHexDocumentPosition();
            auto pos = hexDocument->documentLayout()->hitTest(offsetPos, Qt::HitTestAccuracy::FuzzyHit);

            if (pos != -1) {
                hexCursor.setEndPosition(address, pos);
            }
        }
        else if(asciiCursor.isFocusEnabled) {
            auto offsetPos = event->pos() - getAsciiDocumentPosition();
            auto pos = asciiDocument->documentLayout()->hitTest(offsetPos, Qt::HitTestAccuracy::FuzzyHit);

            if (pos != -1) {
                asciiCursor.setEndPosition(address, pos);
            }
        }

        viewport()->update();
    }

    QAbstractScrollArea::mouseMoveEvent(event);
}

void QHexEditor::paintEvent(QPaintEvent *event) {
    auto start = std::chrono::high_resolution_clock::now();

    if(!isFileOpen()) {
        return;
    }

    if(address != static_cast<uint64_t>(verticalScrollBar()->value() * this->numberOfBytesPerLine())) {
        this->updateContents();
    }

    QPainter painter(viewport());

    paintAddress(painter);
    paintHex(painter);
    paintAscii(painter);

    QAbstractScrollArea::paintEvent(event);
    auto end = std::chrono::high_resolution_clock::now();
    //LOG(INFO) << "paintEvent took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms " << verticalScrollBar()->value();
}

void QHexEditor::paintAddress(QPainter &painter) {
    QAbstractTextDocumentLayout::PaintContext ctx;
    addressDocument->documentLayout()->draw(&painter, ctx);
}

void QHexEditor::paintHex(QPainter &painter) {
    QAbstractTextDocumentLayout::PaintContext ctx;

    if (addressIsInView(hexCursor.startAddress()) && cursorBlinkStatus && hexCursor.isFocusEnabled) {
        ctx.cursorPosition = hexCursor.cursorPosition(address);
    }

    if (hexCursor.isSelection() &&
        (addressIsInView(hexCursor.startAddress()) || addressIsInView(hexCursor.endAddress())) ||
        (this->address >= hexCursor.startAddress() && this->address <= hexCursor.endAddress()) && hexCursor.isFocusEnabled) {
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = QTextCursor(hexDocument);
        selection.cursor.setPosition(qMax(0, hexCursor.startSelectionPosition(address)), QTextCursor::MoveMode::MoveAnchor);
        selection.cursor.setPosition(
                qMin(hexDocument->toPlainText().length() - 1, hexCursor.endSelectionPosition(address)),
                QTextCursor::MoveMode::KeepAnchor);

        selection.format = QTextCharFormat();
        selection.format.setForeground(viewport()->palette().highlightedText());
        selection.format.setBackground(viewport()->palette().highlight());

        ctx.selections.push_back(selection);
    }
    else if (asciiCursor.isSelection() &&
             (addressIsInView(asciiCursor.startAddress()) || addressIsInView(asciiCursor.endAddress())) ||
             (this->address >= asciiCursor.startAddress() && this->address <= asciiCursor.endAddress()) && asciiCursor.isFocusEnabled) {

        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = QTextCursor(hexDocument);
        selection.cursor.setPosition(qMax(0, asciiCursor.startHexSelectionPosition(address, numberOfBytesPerLine())), QTextCursor::MoveMode::MoveAnchor);
        selection.cursor.setPosition(
                qMin(hexDocument->toPlainText().length() - 1, asciiCursor.endHexSelectionPosition(address, numberOfBytesPerLine())),
                QTextCursor::MoveMode::KeepAnchor);

        selection.format = QTextCharFormat();
        selection.format.setForeground(viewport()->palette().highlightedText());
        selection.format.setBackground(viewport()->palette().highlight().color().lighter(120));

        ctx.selections.push_back(selection);
    }

    painter.save();
    painter.translate(getHexDocumentPosition());
    hexDocument->documentLayout()->draw(&painter, ctx);
    painter.restore();
}

void QHexEditor::paintAscii(QPainter &painter) {
    QAbstractTextDocumentLayout::PaintContext ctx;

    if (addressIsInView(asciiCursor.startAddress()) && cursorBlinkStatus && asciiCursor.isFocusEnabled) {
        ctx.cursorPosition = asciiCursor.cursorPosition(address);
    }

    if (asciiCursor.isSelection() &&
        (addressIsInView(asciiCursor.startAddress()) || addressIsInView(asciiCursor.endAddress())) ||
        (this->address >= asciiCursor.startAddress() && this->address <= asciiCursor.endAddress()) && asciiCursor.isFocusEnabled) {
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = QTextCursor(asciiDocument);
        selection.cursor.setPosition(qMax(0, asciiCursor.startSelectionPosition(address)), QTextCursor::MoveMode::MoveAnchor);
        selection.cursor.setPosition(
                qMin(asciiDocument->toPlainText().length() - 1, asciiCursor.endSelectionPosition(address)),
                QTextCursor::MoveMode::KeepAnchor);

        selection.format = QTextCharFormat();
        selection.format.setForeground(viewport()->palette().highlightedText());
        selection.format.setBackground(viewport()->palette().highlight());

        ctx.selections.push_back(selection);
    }
    else if (hexCursor.isSelection() &&
        (addressIsInView(hexCursor.startAddress()) || addressIsInView(hexCursor.endAddress())) ||
        (this->address >= hexCursor.startAddress() && this->address <= hexCursor.endAddress()) && hexCursor.isFocusEnabled) {

        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = QTextCursor(asciiDocument);
        selection.cursor.setPosition(qMax(0, hexCursor.startAsciiSelectionPosition(address, numberOfBytesPerLine())), QTextCursor::MoveMode::MoveAnchor);
        selection.cursor.setPosition(
                qMin(asciiDocument->toPlainText().length() - 1, hexCursor.endAsciiSelectionPosition(address, numberOfBytesPerLine())),
                QTextCursor::MoveMode::KeepAnchor);

        selection.format = QTextCharFormat();
        selection.format.setForeground(viewport()->palette().highlightedText());
        selection.format.setBackground(viewport()->palette().highlight().color().lighter(120));

        ctx.selections.push_back(selection);
    }


    painter.save();
    painter.translate(getAsciiDocumentPosition());
    asciiDocument->documentLayout()->draw(&painter, ctx);
    painter.restore();
}

void QHexEditor::updateContents() {
    if(!isFileOpen()) {
        return;
    }

    auto scrollMaxValue = std::ceil(file->size() / this->numberOfBytesPerLine()) - this->numberOfLines();
    verticalScrollBar()->setRange(0, scrollMaxValue >= 0 ? static_cast<int>(scrollMaxValue + 1) : 0);

    address = static_cast<uint64_t>(verticalScrollBar()->value() * this->numberOfBytesPerLine());

    fillBuffer();

    this->updateAddressText();
    this->updateHexText();
    this->updateAsciiText();
}

void QHexEditor::updateAddressText() {
    std::stringstream ss;

    for(auto y = 0; y < numberOfLines(); y++) {
        ss << std::hex << std::setw(numberOfAddressChars()) << std::setfill('0')
           << std::nouppercase << (address + (y * numberOfBytesPerLine())) << std::endl;

    }

    addressDocument->setDefaultFont(font());
    addressDocument->setPlainText(QString::fromStdString(ss.str()));
}

void QHexEditor::updateHexText() {
    std::stringstream ss;

    for(auto y = 0; y < numberOfLines(); y++) {
        for(auto x = 0; x < numberOfBytesPerLine(); x++) {
            if (address + (y * numberOfBytesPerLine()) + x >= file->size()) {
                break;
            }

            auto ch = buffer.at((this->address - this->bufferAddress) + (y * this->numberOfBytesPerLine()) + x);

            // First we reinterpret to make the value unsigned without changing the underlying bits
            auto byte = reinterpret_cast<unsigned char&>(ch);

            // Then we use static_cast to extend to 32-bits so the stringstream knows it wants to print an integer
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(byte);

            if(x + 1 < numberOfBytesPerLine()) {
                ss << " ";
            }
        }

        ss << std::endl;
    }

    hexDocument->setDefaultFont(font());
    hexDocument->setPlainText(QString::fromStdString(ss.str()));
}


void QHexEditor::updateAsciiText() {
    std::stringstream ss;

    for(auto y = 0; y < numberOfLines(); y++) {
        for(auto x = 0; x < numberOfBytesPerLine(); x++) {
            if (address + (y * numberOfBytesPerLine()) + x >= file->size()) {
                break;
            }

            auto ch = buffer.at((this->address - this->bufferAddress) + (y * this->numberOfBytesPerLine()) + x);

            if (!(ch >= 0x20 && ch <= 0x7e)) {
                ch = '.';
            }

            ss << ch;
        }

        ss << std::endl;
    }

    asciiDocument->setDefaultFont(font());
    asciiDocument->setPlainText(QString::fromStdString(ss.str()));
}

QPointF QHexEditor::getAddressDocumentPosition() {
    return QPointF(0, 0);
}

QPointF QHexEditor::getHexDocumentPosition() {
    QFontMetricsF fontMetrics(font());
    return getAddressDocumentPosition() + QPointF(fontMetrics.averageCharWidth() * (numberOfAddressChars() + 1), 0);
}

QPointF QHexEditor::getAsciiDocumentPosition() {
    QFontMetricsF fontMetrics(font());
    return getHexDocumentPosition() + QPointF(fontMetrics.averageCharWidth() * numberOfBytesPerLine() * 3, 0);
}

/**
 * How many chars are used for the numbers in the address listing
 * @return
 */
const size_t QHexEditor::numberOfAddressChars() {
    return 8;
}

/**
 * Gets the number of bytes to display per line in the ascii/hex view
 * @return
 */
const size_t QHexEditor::numberOfBytesPerLine() {
    return 16;
}

/**
 * Returns the number of bytes displayable in a single hex or ascii view
 * @return
 */
size_t QHexEditor::numberOfBytesInDisplay() {
    return numberOfBytesPerLine() * numberOfLines();
}

/**
 * Gets the number of lines that can be displayed in this widget, based on the heights of this widget and font
 * @return
 */
size_t QHexEditor::numberOfLines() {
    QFontMetricsF fontMetrics(font());

    auto height = viewport()->height();
    height -= fontMetrics.leading();

    auto numLines = height / qCeil(fontMetrics.height() + fontMetrics.leading());

    return static_cast<size_t>(qCeil(numLines));
}

bool QHexEditor::addressIsInView(uint64_t address) {
    return address >= this->address && address < (this->address + (numberOfBytesPerLine() * numberOfLines()));
}

void QHexEditor::dragEnterEvent(QDragEnterEvent *event) {
    event->accept();
}

void QHexEditor::dragMoveEvent(QDragMoveEvent *event) {
    event->accept();
}

void QHexEditor::dragLeaveEvent(QDragLeaveEvent *event) {
    event->ignore();
}

void QHexEditor::dropEvent(QDropEvent *event) {
    for(auto &url : event->mimeData()->urls()) {
        openFile(url.toLocalFile());
        return;
    }
}

bool QHexEditor::isFileOpen() {
    return (this->file != nullptr) && file->isOpen();
}

void QHexEditor::closeFile() {
    if(isFileOpen()) {
        file->close();
    }

    delete file;
    file = nullptr;
}

bool QHexEditor::openFile(const QString &filePath) {
    if(isFileOpen()) {
        closeFile();
    }

    file = new QFile(filePath);
    if(!file->open(QFile::OpenModeFlag::ReadOnly)) {
        closeFile();
        return false;
    }

    if(!file->isOpen() || !file->isReadable()) {
        closeFile();
        return false;
    }

    return true;
}

/**
 * Fill the internal buffer with data from the stream based on the current address displayed in the viewport
 */
void QHexEditor::fillBuffer() {
    if (buffer.size() == 0 || address < bufferAddress ||
        address + numberOfBytesInDisplay() > bufferAddress + buffer.size()) {

        if(!isFileOpen()) {
            return;
        }

        auto bytesToRead = std::min<size_t>(bufferMaxSize, file->size());

        if(bytesToRead >= file->size()) {
            bufferAddress = 0;
            bytesToRead = file->size();
        }
        else if(bytesToRead > (file->size() - bufferAddress)) {
            bufferAddress = file->size() - bytesToRead - 1;
        }
        else {
            bufferAddress = address;
        }

        file->seek(bufferAddress);
        buffer = file->read(bytesToRead);
    }
}