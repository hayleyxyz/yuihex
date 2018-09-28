#include <QApplication>
#include "MainWindow.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    QApplication application(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
}