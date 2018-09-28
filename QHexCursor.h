//
// Created by yuikonnu on 28/09/2018.
//

#ifndef YUIHEX_QHEXCURSOR_H
#define YUIHEX_QHEXCURSOR_H

#include <QObject>

class QHexCursor : public QObject {
    Q_OBJECT

public:
    explicit QHexCursor(QObject *parent = nullptr);
    quint64 startAddress();
    quint64 endAddress();
    bool isSelection();
    bool isReverseSelection();
    void setForceSelection(bool forceSelection);
    bool isSelecting();
    void setSelecting(bool selecting);
    void setStartAddress(quint64 address);
    void setEndAddress(quint64 address);
    void setStartHexCharIndex(int index);
    void setEndHexCharIndex(int index);
    int startHexCharIndex();
    int endHexCharIndex();
    int startHexTextPosition(int bytesPerLine);
    int endHexTextPosition(int bytesPerLine);
    int addressToHexTextPosition(quint64 address, int bytesPerLine);


protected:
    quint64 _startAddress = 0;
    quint64 _endAddress = 0;
    int _startHexPartial = 0;
    int _endHexPartial = 0;
    bool _isSelecting = false;
    bool _forceSelection = false;
};


#endif //YUIHEX_QHEXCURSOR_H
