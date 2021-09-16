#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("File Manager");
    a.setWindowIcon(QIcon(":/Images/App.ico"));
    MainWindow w;

    w.show();
    return a.exec();
}
