#include "qhexeditor.h"
#include "easylogging++.h"
#include <iomanip>
#include <QPainter>
#include <QScrollBar>
#include <QMimeData>
#include <QGuiApplication>
#include <QTimer>

INITIALIZE_EASYLOGGINGPP

QHexEditor::QHexEditor(QWidget *parent) :
    QAbstractScrollArea(parent)
{
    // Timer for cursor blink
    this->startTimer(600, Qt::CoarseTimer);
}

void QHexEditor::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);

    cursorBlink = !cursorBlink;
    viewport()->update();
}

void QHexEditor::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    auto start = std::chrono::high_resolution_clock::now();

    QPainter painter(viewport());

    if(this->stream == nullptr) {
        return;
    }

    auto scrollMaxValue = std::ceil(this->stream->length() / this->numberOfBytesPerLine()) - this->numberOfLines();
    LOG(INFO) << "scrollMaxValue: " << scrollMaxValue;
    verticalScrollBar()->setRange(0, scrollMaxValue >= 0 ? static_cast<int>(scrollMaxValue + 1) : 0);

    this->address = static_cast<addr_t>(verticalScrollBar()->value() * this->numberOfBytesPerLine());

    this->fillBuffer();

    painter.fillRect(this->frameRect(), QColor(255, 255, 255));

    this->drawSelections(painter);
    this->drawAddressView(painter);
    this->drawHexView(painter);
    this->drawAsciiView(painter);
    this->drawCursor(painter);

    auto end = std::chrono::high_resolution_clock::now();
    LOG(INFO) << "paintEvent took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
}

void QHexEditor::drawCursor(QPainter &painter) {
    auto addr = std::max(this->textCursor.addr - this->address, 0ll);

    if(addr > 0) {
        auto y = addr / this->numberOfBytesPerLine();
        auto x = addr % this->numberOfBytesPerLine();
        auto pos = this->getHexTextPosition(x, y);

        auto rect = QRect(
                pos.x(),
                pos.y() - charAscent(),
                1,
                charAscent());

        painter.fillRect(rect, painter.pen().brush());
    }
}

void QHexEditor::keyPressEvent(QKeyEvent *event) {
    if(event->modifiers() == Qt::KeyboardModifier::ShiftModifier) {
        QAbstractScrollArea::keyPressEvent(event);
    }

    QAbstractScrollArea::keyPressEvent(event);
}

void QHexEditor::mousePressEvent(QMouseEvent *event) {
    QPoint coord;

    if(this->getHexViewBoundingBox().contains(event->x(), event->y())) {
        coord = this->getHexCharCoordForPosition(event->x(), event->y());
        this->currentSelection.window = Selection::Window::Hex;
    }
    else if(this->getAsciiViewBoundingBox().contains(event->x(), event->y())) {
        coord = this->getAsciiCharCoordForPosition(event->x(), event->y());
        this->currentSelection.window = Selection::Window::Ascii;
    }

    this->currentSelection.end = static_cast<addr_t>(this->address + std::min((coord.y() * this->numberOfBytesPerLine()) + coord.x(), this->numberOfBytesInDisplay() - 1));

    if(!(this->currentSelection.active && QGuiApplication::keyboardModifiers() == Qt::KeyboardModifier::ShiftModifier)) {
        this->currentSelection.start = this->currentSelection.end;
    }

    this->textCursor.addr = this->currentSelection.end;

    viewport()->update();

    QAbstractScrollArea::mousePressEvent(event);
}

void QHexEditor::keyReleaseEvent(QKeyEvent *event) {
    QAbstractScrollArea::keyReleaseEvent(event);
}

void QHexEditor::mouseMoveEvent(QMouseEvent *event) {
    LOG(INFO) << "mouseMoveEvent";

    auto buttons = event->buttons();
    if(buttons == Qt::MouseButton::LeftButton) {
        QPoint coord;
        if(this->getHexViewBoundingBox().contains(event->x(), event->y())) {
            coord = this->getHexCharCoordForPosition(event->x(), event->y());
        }
        else if(this->getAsciiViewBoundingBox().contains(event->x(), event->y())) {
            coord = this->getAsciiCharCoordForPosition(event->x(), event->y());
        }
        else {
            return;
        }

        this->currentSelection.end = static_cast<addr_t>(this->address + std::min((coord.y() * this->numberOfBytesPerLine()) + coord.x(), this->numberOfBytesInDisplay() - 1));

        auto numberOfBytesSelected = (std::max(this->currentSelection.start, this->currentSelection.end) -
                                      std::min(this->currentSelection.start, this->currentSelection.end));

        // How many pixels across the x axis the cursor is relative to the coord
        auto subCharPixels = (event->x() - this->getHexViewBoundingBox().x()) - (coord.x() * this->charWidth() * 2 * 1.5);

        if(numberOfBytesSelected >= 1) {
            this->currentSelection.active = true;
        }
        else if(numberOfBytesSelected == 0 && subCharPixels >= this->charWidth() * 0.5) {
            this->currentSelection.active = true;
        }
        else {
            this->currentSelection.active = false;
        }

        viewport()->update();
    }

    QAbstractScrollArea::mouseMoveEvent(event);
}

void QHexEditor::wheelEvent(QWheelEvent *event) {
    QAbstractScrollArea::wheelEvent(event);
}

bool QHexEditor::event(QEvent *event) {
    if(event->type() == QEvent::Scroll) {
        return QAbstractScrollArea::event(event);
    }
    else if(event->type() == QEvent::MouseButtonPress) {
        return QAbstractScrollArea::event(event);
    }
    else if(event->type() == QEvent::Show) {

        return QAbstractScrollArea::event(event);
    }
    else if(event->type() == QEvent::Paint) {
        return QAbstractScrollArea::event(event);
    }

    return QAbstractScrollArea::event(event);
}

QPoint QHexEditor::getHexCharCoordForPosition(int x, int y) {
    auto hexbbox = this->getHexViewBoundingBox();

    auto coordX = ((x - hexbbox.x()) + (this->charWidth() * 0.5)) / (this->charWidth() * 2 * 1.5);
    auto coordY = (y - hexbbox.y()) / this->charAscent();

    return QPoint(static_cast<int>(coordX), coordY);
}

QPoint QHexEditor::getAsciiCharCoordForPosition(int x, int y) {
    auto bbox = this->getAsciiViewBoundingBox();

    auto coordX = (x - bbox.x()) / this->charWidth();
    auto coordY = (y - bbox.y()) / this->charAscent();

    return QPoint(coordX, coordY);
}

QRect QHexEditor::getAsciiViewBoundingBox() {
    auto start = this->getAsciiTextPosition(0, 0);
    auto end = this->getAsciiTextPosition(this->numberOfBytesPerLine() - 1, this->numberOfLines() - 1);

    return QRect(
            start.x(),
            start.y() - charAscent(),
            (end.x() - start.x()) + charWidth(),
            end.y() - start.y()
    );
}

QRect QHexEditor::getHexViewBoundingBox() {
    auto hexStart = this->getHexTextPosition(0, 0);
    auto hexEnd = this->getHexTextPosition(this->numberOfBytesPerLine() - 1, this->numberOfLines() - 1);

    return QRect(
            QPoint(
                hexStart.x(),
                hexStart.y() - charAscent()
            ),
            QPoint(
                    hexEnd.x() + (charWidth() * 2),
                    hexEnd.y()
            )
    );
}

void QHexEditor::drawSelections(QPainter &painter) {
    if(this->currentSelection.active) {
        this->drawHexSelections(painter);
        this->drawAsciiSelections(painter);
    }
}

void QHexEditor::drawAsciiSelections(QPainter &painter) {
    auto bytesStart = std::min(this->currentSelection.start, this->currentSelection.end) - this->address;
    auto bytesEnd = std::max(this->currentSelection.start, this->currentSelection.end) - this->address;

    bytesStart = std::max(bytesStart, -1ll);
    bytesEnd = std::max(bytesEnd, -1ll);

    if(bytesEnd > 0 && (bytesEnd - bytesStart) >= 0) {
        for (auto i = bytesStart; i <= bytesEnd; i++) {

            auto hiY = i / this->numberOfBytesPerLine();
            auto hiX = i % this->numberOfBytesPerLine();

            auto hex = this->getAsciiTextPosition(hiX, hiY);
            auto rect = QRect(
                    hex.x(),
                    hex.y() - charAscent(),
                    charWidth(),
                    charAscent());

            painter.fillRect(rect, this->currentSelection.window == Selection::Window::Ascii ? this->primarySelectionColour() : this->secondarySelectionColour());
        }
    }
}

void QHexEditor::drawHexSelections(QPainter &painter) {
    auto bytesStart = std::min(this->currentSelection.start, this->currentSelection.end) - this->address;
    auto bytesEnd = std::max(this->currentSelection.start, this->currentSelection.end) - this->address;

    bytesStart = std::max(bytesStart, -1ll);
    bytesEnd = std::max(bytesEnd, -1ll);

    if(bytesEnd > 0 && (bytesEnd - bytesStart) >= 0) {
        for (auto i = bytesStart; i <= bytesEnd; i++) {

            auto hiY = i / this->numberOfBytesPerLine();
            auto hiX = i % this->numberOfBytesPerLine();

            auto hex = this->getHexTextPosition(hiX, hiY);
            auto rect = QRect(
                    hex.x(),
                    hex.y() - charAscent(),
                    // Don't apply padding to bytes at the end of the row
                    charWidth() * (hiX == (this->numberOfBytesPerLine() - 1) || i == bytesEnd ? 2 : 3),
                    charAscent());

            painter.fillRect(rect, this->currentSelection.window == Selection::Window::Hex ? this->primarySelectionColour() : this->secondarySelectionColour());
        }
    }
}

/**
 * Draw ascii view of data
 */
void QHexEditor::drawAsciiView(QPainter &painter) {
    for(auto y = 0u; y < this->numberOfLines(); y++) {
        for(auto x = 0u; x < this->numberOfBytesPerLine(); x++) {
            auto byte = buffer[(this->address - this->bufferAddress) + (y * this->numberOfBytesPerLine()) + x];

            auto pos = this->getAsciiTextPosition(x, y);
            painter.drawText(pos.x(), pos.y(), this->formatAsciiByteText(byte));
        }
    }
}

/**
 * Format hex byte for display in hex view
 * @param byte
 * @return
 */
QString QHexEditor::formatAsciiByteText(uint8_t byte) {
    auto ch = static_cast<QChar>(byte);

    if(ch == QChar::SpecialCharacter::Null) {
        ch = '.';
    }

    return QString(ch);
}

/**
 * Gets position to draw text in ascii view
 * @param x
 * @param y
 * @return
 */
QPoint QHexEditor::getAsciiTextPosition(int x, int y) {
    auto hexPos = this->getHexTextPosition(static_cast<int>(this->numberOfBytesPerLine()), 0);

    return QPoint(
            hexPos.x() + this->charWidth() + (x * this->charWidth()),
            this->charAscent() + (y * this->charAscent())
    );
}

/**
 * Draw hex view
 * @param painter
 */
void QHexEditor::drawHexView(QPainter &painter) {
    for(auto y = 0u; y < this->numberOfLines(); y++) {
        for(auto x = 0u; x < this->numberOfBytesPerLine(); x++) {
            auto byte = buffer[(this->address - this->bufferAddress) + (y * this->numberOfBytesPerLine()) + x];

            auto pos = this->getHexTextPosition(x, y);
            painter.drawText(pos.x(), pos.y(), this->formatHexByteText(byte));
        }
    }
}

/**
 * Draw vertical address view
 * @param painter
 */
void QHexEditor::drawAddressView(QPainter &painter) {
    for(auto y = 0u; y < this->numberOfLines(); y++) {
        auto pos = this->getAddressTextPosition(y);
        painter.drawText(pos.x(), pos.y(), this->formatAddressText(address + (y * this->numberOfBytesPerLine())));
    }
}

/**
 * Format hex byte for display in hex view
 * @param byte
 * @return
 */
QString QHexEditor::formatHexByteText(uint8_t byte) {
    int byteInt = byte;

    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (this->isUppercaseHex() ? std::uppercase : std::nouppercase) << byteInt;
    return QString::fromStdString(ss.str());
}

/**
 * Gets position of hex byte in hex view based on x/y
 * @param x
 * @param y
 * @return
 */
QPoint QHexEditor::getHexTextPosition(int x, int y) {
    auto hexStart = this->charWidth() + (this->numberOfAddressChars() * this->charWidth());

    return QPoint(
            static_cast<int>(hexStart + this->charWidth() + (((x * charWidth()) * 2) * 1.5)),
            this->charAscent() + (y * this->charAscent())
    );
}

/**
 * Gets number of chars in each address line
 * @return
 */
int QHexEditor::numberOfAddressChars() {
    return 8;
}

/**
 * Format an address for display as text
 * @param address
 * @return
 */
QString QHexEditor::formatAddressText(addr_t address) {
    auto charCount = this->numberOfAddressChars();

    std::stringstream ss;
    ss << std::hex << std::setw(charCount) << std::setfill('0') << (this->isUppercaseHex() ? std::uppercase : std::nouppercase) << address;
    return QString::fromStdString(ss.str());
}

/**
 * Gets the position of text for the address bar view
 * @param y
 * @return
 */
QPoint QHexEditor::getAddressTextPosition(int y) {
    return QPoint(
            this->charWidth(),
            this->charAscent() + (y * this->charAscent())
    );
}

int QHexEditor::charAscent() {
    if(this->_charAscent == 0) {
        QFontMetrics fm(this->font());
        this->_charAscent = fm.ascent();
    }

    return this->_charAscent;
}

/**
 * Gets the width of a single character
 * TODO: Is this the same for all chars?
 * @return
 */
int QHexEditor::charWidth() {
    if(this->_charWidth == 0) {
        QFontMetrics fm(this->font());
        this->_charWidth = fm.averageCharWidth();
    }

    return this->_charWidth;
}

/**
 * Gets the height of a single character
 * @return
 */
int QHexEditor::charHeight() {
    if(this->_charHeight == 0) {
        QFontMetrics fm(this->font());
        this->_charHeight = fm.height();
    }

    return this->_charHeight;
}

/**
 * Returns the number of bytes displayable in a single hex or ascii view
 * @return
 */
size_t QHexEditor::numberOfBytesInDisplay() {
    return this->numberOfBytesPerLine() * this->numberOfLines();
}

/**
 * Gets the number of bytes to display per line in the ascii/hex view
 * @return
 */
size_t QHexEditor::numberOfBytesPerLine() {
    return 16;
}

/**
 * Gets the number of lines to display in the ascii/hex view
 * @return
 */
size_t QHexEditor::numberOfLines() {
    return static_cast<size_t>(std::min(
            ::floor(viewport()->height() / charAscent()),
            ::ceil(static_cast<double>(this->stream->length()) / this->numberOfBytesPerLine())
    ));
}

/**
 * Fill the internal buffer with data from the stream based on the current address displayed in the viewport
 */
void QHexEditor::fillBuffer() {
    if (this->buffer == nullptr || this->address < this->bufferAddress ||
            this->address + this->numberOfBytesInDisplay() > this->bufferAddress + this->bufferSize) {
        if(this->buffer == nullptr) {
            buffer = new uint8_t[this->bufferMaxSize];
        }

        if(this->stream == nullptr) {
            return;
        }

        auto streamLength = this->stream->length();

        this->bufferSize = std::min<size_t>(this->bufferMaxSize, streamLength);

        if(this->bufferSize >= streamLength) {
            this->bufferAddress = 0;
            this->bufferSize = streamLength;
        }
        else if(this->bufferSize > (streamLength - this->bufferAddress)) {
            this->bufferAddress = streamLength - this->bufferSize - 1;
        }
        else {
            this->bufferAddress = this->address;
        }

        LOG(INFO) << "bufferAddress: " << bufferAddress;

        this->stream->seek(bufferAddress, Stream::Start);
        this->stream->read(this->buffer, this->bufferSize);
    }
}

/**
 * Specifies if the hex in the address/hex view is displayed using uppercase chars
 * @return
 */
bool QHexEditor::isUppercaseHex() const {
    return this->uppercaseHex;
}

/**
 * Sets the value to return from isUppercaseHex()
 * @param value
 */
void QHexEditor::setUppercaseHex(bool value) {
    this->uppercaseHex = value;
}

QColor QHexEditor::primarySelectionColour() {
    return viewport()->palette().color(QPalette::Highlight);
}

QColor QHexEditor::secondarySelectionColour() {
    return viewport()->palette().color(QPalette::Highlight).lighter(120);
}

bool QHexEditor::openFile(const QString &file) {
    if(this->stream != nullptr) {
        this->stream->close();
        delete this->stream;
        this->stream = nullptr;
    }

    if(this->buffer != nullptr) {
        delete[] this->buffer;
        this->buffer = nullptr;
    }

    this->stream = new FileStream();
    auto result = this->stream->open(file.toStdString(), FileStream::Mode::OpenRead);

    if(result) {
        emit fileOpened(file);
    }

    return result;
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
        LOG(INFO) << url.toLocalFile().toStdString();
        this->openFile(url.toLocalFile());
        return;
    }
}

void QHexEditor::closeFile() {
    if(this->stream != nullptr) {
        this->stream->close();
        delete this->stream;
        this->stream = nullptr;
    }
}
