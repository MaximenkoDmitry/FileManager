#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include<QDesktopServices>
#include<QListWidgetItem>
#include <dirent.h>
#include <string>
#include<QUrl>
#include <QMessageBox>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<functions.h>
#include<QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pathButton_clicked();

    void on_searchButton_clicked();

    void on_actionExit_triggered();

    void sort();

    void showFiles(char* path, int flag);

    void on_actionAbout_the_program_triggered();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionCreate_catalog_triggered();

    void on_actionDelete_triggered();

    void on_actionRename_triggered();

    void on_actionFile_Information_triggered();

    void on_actionPaste_triggered();
    void show_dir_content(char *path, char* name);

    void on_homeButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
