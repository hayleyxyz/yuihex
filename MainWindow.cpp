//
// Created by yuikonnu on 24/09/2018.
//

#include "MainWindow.h"
#include "QHexEditor.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    this->resize(800, 600);

    auto centralWidget = new QWidget(this);
    centralWidget->setEnabled(true);
    this->setCentralWidget(centralWidget);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);

    auto horizontalLayout = new QHBoxLayout(centralWidget);
    {
        auto hexEditor = new QHexEditor(centralWidget);
        hexEditor->setAcceptDrops(true);
        hexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        //hexEditor->connect(hexEditor, SIGNAL(fileOpened(QString)), this, SLOT(fileWasOpened(QString)));
        //hexEditor->connect(hexEditor, SIGNAL(cursorChanged(Cursor)), this, SLOT(hexCursorChanged(Cursor)));
        horizontalLayout->addWidget(hexEditor);

        QFont font;
        font.setFamily(QStringLiteral("Source Code Pro"));
        font.setPointSize(12);
        hexEditor->setFont(font);
    }
}
