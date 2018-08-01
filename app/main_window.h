//
// Created by Oscar Berry on 22/07/2018.
//

#ifndef PROJECT_MAIN_WINDOW_H
#define PROJECT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <qhexeditor.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

protected:
    QFileDialog *fileDialog;
    QHexEditor *hexEditor;

protected slots:
    void browseForFile();
    void closeFile();
    void fileSelected(const QString &file);
    void fileWasOpened(const QString &file);
};


#endif //PROJECT_MAIN_WINDOW_H
