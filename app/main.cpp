#include <QApplication>
#include "easylogging++.h"
#include "main_window.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    LOG(INFO) << "Start";

    QApplication application(argc, argv);
    MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}