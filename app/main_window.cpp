//
// Created by yuikonnu on 22/07/2018.
//

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <qhexeditor.h>
#include "main_window.h"
#include "filesystem.h"
#include "debug.h"
#include <magic.h>


MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags) {
    this->resize(800, 600);

    auto centralWidget = new QWidget(this);
    centralWidget->setEnabled(true);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy);

    auto horizontalLayout = new QHBoxLayout(centralWidget);

    {
        this->hexEditor = new QHexEditor(centralWidget);
        this->hexEditor->setAcceptDrops(true);
        this->hexEditor->connect(hexEditor, SIGNAL(fileOpened(QString)), this, SLOT(fileWasOpened(QString)));
        this->hexEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        QFont font;
        font.setFamily(QStringLiteral("Source Code Pro"));
        font.setPointSize(12);
        this->hexEditor->setFont(font);
    }

    horizontalLayout->addWidget(this->hexEditor);

    auto verticalLayout = new QVBoxLayout();
    horizontalLayout->addLayout(verticalLayout);

    {
        auto magicGroupBox = new QGroupBox(centralWidget);
        magicGroupBox->setTitle(tr("Magic"));
        magicGroupBox->setMaximumHeight(80);
        magicGroupBox->setFixedWidth(250);

        auto magicLayout = new QVBoxLayout();
        magicGroupBox->setLayout(magicLayout);

        auto magicLabel = new QLabel(magicGroupBox);
        magicLabel->setText(tr("no file open"));
        magicLabel->setWordWrap(true);
        magicLabel->setObjectName(QStringLiteral("magicLabel"));
        magicLayout->addWidget(magicLabel);
        verticalLayout->addWidget(magicGroupBox);
    }
    {
        auto groupBox = new QGroupBox(centralWidget);
        groupBox->setTitle(QStringLiteral("Data Types"));
        groupBox->setFixedWidth(250);
        verticalLayout->addWidget(groupBox);

        auto formLayout = new QFormLayout;
        formLayout->addRow(new QLabel(tr("Big endian")), new QCheckBox);
        formLayout->addRow(new QLabel(tr("int8")), new QLineEdit);
        formLayout->addRow(new QLabel(tr("int16")), new QLineEdit);
        formLayout->addRow(new QLabel(tr("int32")), new QLineEdit);
        formLayout->addRow(new QLabel(tr("int64")), new QLineEdit);
        formLayout->setFormAlignment(Qt::AlignmentFlag::AlignJustify);
        groupBox->setLayout(formLayout);
    }

    this->setCentralWidget(centralWidget);

    auto menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 800, 22));
    {
        auto menu = new QMenu(menuBar);
        menu->setTitle(QStringLiteral("File"));
        {
            auto openAction = new QAction(this);
            openAction->setText(QStringLiteral("Open..."));
            openAction->setShortcut(QKeySequence::Open);
            this->connect(openAction, SIGNAL(triggered()), this, SLOT(browseForFile()));
            menu->addAction(openAction);
        }
        {
            auto closeAction = new QAction(this);
            closeAction->setText(QStringLiteral("Close"));
            closeAction->setShortcut(QKeySequence::Close);
            this->connect(closeAction, SIGNAL(triggered()), this, SLOT(closeFile()));
            menu->addAction(closeAction);
        }

        menuBar->addAction(menu->menuAction());
    }

    menuBar->setNativeMenuBar(true);
    this->setMenuBar(menuBar);

    {
        auto statusBar = new QStatusBar(this);
        auto label = new QLabel(statusBar);
        label->setText(QStringLiteral("benis"));
        this->setStatusBar(statusBar);
    }

    {
        this->fileDialog = new QFileDialog(this);
        //this->fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
        this->fileDialog->setFileMode(QFileDialog::ExistingFile);
        this->connect(this->fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileSelected(QString)));
    }
}


void MainWindow::browseForFile() {
    this->fileDialog->show();
}

void MainWindow::fileSelected(const QString &file) {
    if(filesystem::is_dir(file.toStdString())) {
        auto msg = new QMessageBox(QMessageBox::Critical, QStringLiteral("Error"),
                                   QStringLiteral("Input file is a directory!"));
        msg->show();
        return;
    }

    this->hexEditor->openFile(file);
}

void MainWindow::closeFile() {
    this->hexEditor->closeFile();
}

void MainWindow::fileWasOpened(const QString &file) {

    auto label = this->findChild<QLabel *>(QStringLiteral("magicLabel"));

    auto magic = magic_open(MAGIC_DEBUG | MAGIC_COMPRESS);
    magic_load(magic, nullptr);
    auto description = magic_file(magic, file.toStdString().c_str());
    if(description == nullptr) {
        auto error = magic_error(magic);
        label->setText(QString(error));
    }
    else {
        label->setText(QString(description));
    }
}
