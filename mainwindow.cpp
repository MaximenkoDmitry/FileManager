#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

int flag_find = 0;
char* homePath = convertStringToChar("/");

#define FILE_TYPE 1
#define CATALOG_TYPE 0

void MainWindow::sort(){
    static Qt::SortOrder order = Qt::AscendingOrder;
    ui->listWidget->sortItems(order);
}

void MainWindow::showFiles(char* path,int type){
    flag_find = 0;
    DIR* dirs = opendir(path);
    if(dirs!=NULL){
        struct dirent *f_cur;
        struct stat info;
        ui->pathInput->setText(path);
        while ((f_cur=readdir(dirs))!=NULL){
            char* path = convertQStringToChar(ui->pathInput->text());
            path = addToPath(path,homePath);
            path = addToPath(path,f_cur->d_name);
            stat(path,&info);
            string name(f_cur->d_name);
            if(type == CATALOG_TYPE&&name!="."&&((f_cur->d_name[0]=='.'&&f_cur->d_name[1]=='.')||(f_cur->d_name[0]!='.'))&&checkCatalogType(&info)){
                ui->listWidget->addItem(new QListWidgetItem(QIcon("/home/dmitry/folder.png"),f_cur->d_name));
            }
            else{
                if(type == FILE_TYPE&&f_cur->d_name[0]!='.'&&checkFileType(&info)){
                    ui->listWidget->addItem(new QListWidgetItem(QIcon("/home/dmitry/file.png"),f_cur->d_name));
                }
            }
        }
        if(type == CATALOG_TYPE){
            sort();
        }
        closedir(dirs);
    }
    else{
        if(type == FILE_TYPE)
            QMessageBox::warning(this, "Attention!","The path is specified incorrectly!");
        char* path = convertStringToChar("/");
        ui->listWidget->clear();
        showFiles(path,CATALOG_TYPE);
        showFiles(path,FILE_TYPE);

    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pathInput->setText("/");
    ui->listWidget->setWrapping(true);
    DIR* dirs = opendir("/");
    if(dirs!=NULL){
        ui->listWidget->clear();
        showFiles(homePath,CATALOG_TYPE);
        showFiles(homePath,FILE_TYPE);
        closedir(dirs);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


//input path and open
void MainWindow::on_pathButton_clicked()
{
    if(ui->pathInput->text() == NULL){
        QMessageBox::warning(0, "Warning",  "Path is empty");
        ui->pathInput->setText("/");
    }
    ui->listWidget->clear();
    showFiles(convertQStringToChar(ui->pathInput->text()),CATALOG_TYPE);
    showFiles(convertQStringToChar(ui->pathInput->text()),FILE_TYPE);
}


//find files/catalogs
void MainWindow::on_searchButton_clicked()
{
    QString qNameFind = ui->searchInput->text();
    if(qNameFind == NULL){
        QMessageBox::warning(0, "Warning",  "Find is empty");
        return;
    }
    char* nameFind = convertQStringToChar(qNameFind);
    ui->listWidget->clear();
    show_dir_content(convertStringToChar("/home"),nameFind);
    flag_find = 1;
    ui->searchInput->setText("");
}

void MainWindow::show_dir_content(char *path, char* name){
    DIR *d = opendir(path);
    if (d == NULL)
        return;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL)
    {
        char fullPath[300];
        sprintf(fullPath, "%s/%s", !strcmp(path, "/") ? "" : path, dir->d_name);

        if (!strcmp(name, dir->d_name))
        {
            cout<<fullPath<<endl;
            ui->listWidget->addItem(new QListWidgetItem(QIcon("/home/dmitry/url.png"),fullPath));
        }

        if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) // if it is a directory
        {
            show_dir_content(fullPath, name);
        }
    }
    closedir(d);
}


//exit program
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


//about the program
void MainWindow::on_actionAbout_the_program_triggered()
{
    QMessageBox::information(0, "Information",  "File Manager\n"
                                                "Version 1.0\n"
                                                "Created by Maximenko Dmitry\n"
                                                "BSUIR, VMSiS, 950506\n");
}


//open catalog/file
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(flag_find == 1){
        char* path = convertQStringToChar(item->text());
        string url(path);
        struct stat info;
        stat(path,&info);
        if(checkFileType(&info)){
            QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
            return;
        }
        else{
            if(checkCatalogType(&info)){
                ui->listWidget->clear();
                showFiles(convertStringToChar(url),CATALOG_TYPE);
                showFiles(convertStringToChar(url),FILE_TYPE);
                return;
            }
        }
    }
    QString qpath = ui->pathInput->text();
    QByteArray ba = qpath.toLatin1();
    char *path = ba.data();
    std::string Path(path);
    QString line = item->text();
    ba = line.toLatin1();
    char *name = ba.data();
    std::string Name(name);
    if(Name == "."){
        return;
    }
    if(Name == ".."){
        if(Path == "/"){
            return;
        }
        else{
            char* new_path = NULL;
            new_path = deleteNameFromPath(path);
            ui->listWidget->clear();
            showFiles(new_path,CATALOG_TYPE);
            showFiles(new_path,FILE_TYPE);
            return;
        }
    }
    else{
        if(Path == "/"){
            path = addToPath(path,name);
            ui->listWidget->clear();
            showFiles(path,CATALOG_TYPE);
            showFiles(path,FILE_TYPE);
            return;
        }
        path = addToPath(path,homePath);
        path = addToPath(path,name);
        DIR* dirs = opendir(path);
        if(dirs == NULL){
            QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
            return;
        }
        ui->listWidget->clear();
        showFiles(path,CATALOG_TYPE);
        showFiles(path,FILE_TYPE);
    }
    return;
}

//create a new catalog
void MainWindow::on_actionCreate_catalog_triggered()
{
    bool bOk;
    QString str = QInputDialog::getText( 0,
                                         "Input",
                                         "Name of new catalog:",
                                         QLineEdit::Normal,
                                         "Catalog",
                                         &bOk
                                        );

    if (!bOk) {
        return;
    }
    std::string newDir = str.toUtf8().constData();
    QString qpath = ui->pathInput->text();
    QByteArray ba = qpath.toLatin1();
    std::string Path(ba.data());
    if(Path == "/"||Path == "/home"){
        QMessageBox::warning(this, "Attention!","Not create new catalog in this path!");
        return;
    }
    Path = Path+"/";
    Path = Path + newDir;
    char *path = convertStringToChar(Path);
    if (mkdir(path, 0777) == -1)
        QMessageBox::warning(this, "Attention!",
                             "An error occurred while creating the folder!"
                             "Perhaps a directory with this name already exists!");
    delete [] path;
    ui->listWidget->clear();
    showFiles(ba.data(),CATALOG_TYPE);
    showFiles(ba.data(),FILE_TYPE);
    return;
}

//delete catalog or file
void MainWindow::on_actionDelete_triggered()
{
    QString qpath = ui->pathInput->text();
    QByteArray ba = qpath.toLatin1();
    char *path = ba.data();
    std::string Path(path);
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item == NULL){
        QMessageBox::warning(0, "Warning",  "File not selected!");
        return;
    }
    QString line = item->text();
    ba = line.toLatin1();
    char *name = ba.data();
    std::string Name(name);
    if(Name == "."||Name == ".."){
        return;
    }
    else{
        char* pathParent = addToPath(convertStringToChar(""), path);
        char* pathForDelete = addToPath(convertStringToChar(""),path);
        pathForDelete = addToPath(pathForDelete,homePath);
        pathForDelete = addToPath(pathForDelete,name);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete","Are you sure you want to delete?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            remove(pathForDelete);
            ui->listWidget->clear();
            showFiles(pathParent,CATALOG_TYPE);
            showFiles(pathParent,FILE_TYPE);
        }
    }
    return;

}


//rename catalog or file
void MainWindow::on_actionRename_triggered()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item == NULL){
        QMessageBox::warning(0, "Warning",  "File not selected!");
        return;
    }
    bool bOk;
    QString str = QInputDialog::getText( 0,
                                         "Rename",
                                         "New name: ",
                                         QLineEdit::Normal,
                                         item->text(),
                                         &bOk
                                        );

    if (!bOk) {
        return;
    }
    std::string NewName = str.toUtf8().constData();
    std::string OldName = item->text().toUtf8().constData();
    QString qpath = ui->pathInput->text();
    QByteArray ba = qpath.toLatin1();
    std::string Path(ba.data());
    NewName = Path + "/" + NewName;
    OldName = Path + "/" + OldName;
    char* newName = convertStringToChar(NewName);
    char* oldName = convertStringToChar(OldName);
    if(rename(oldName,newName)){
        QMessageBox::warning(this, "Attention!","ERROR: error of rename file!");
    }
    delete [] newName;
    delete [] oldName;
    ui->listWidget->clear();
    showFiles(ba.data(),CATALOG_TYPE);
    showFiles(ba.data(),FILE_TYPE);
    return;
}


void MainWindow::on_actionFile_Information_triggered()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item == NULL){
        QMessageBox::warning(0, "Warning",  "File not selected!");
        return;
    }
    char* path = addToPath(convertQStringToChar(ui->pathInput->text()),addToPath(homePath,convertQStringToChar(item->text())));
    QMessageBox::information(0, "File Information",  getInformationAboutFile(path,convertQStringToChar(item->text())));
}


//move file
void MainWindow::on_actionPaste_triggered()
{
    QListWidgetItem* item = ui->listWidget->currentItem();
    if(item == NULL){
        QMessageBox::warning(0, "Warning",  "File not selected!");
        return;
    }
    char* path = convertQStringToChar(ui->pathInput->text());
    cout<<"PATH: "<<path<<endl;
    char* oldPath = addToPath(path,homePath);
    oldPath = addToPath(oldPath,convertQStringToChar(item->text()));
   // char* oldPath = addToPath(path,addToPath(homePath,convertQStringToChar(item->text())));
    cout<<"OLD: "<<oldPath<<endl;
    bool bOk;
    QString str = QInputDialog::getText( 0,
                                         "Input",
                                         "Path for moving: ",
                                         QLineEdit::Normal,
                                         "/home/dmitry",
                                         &bOk
                                        );

    if (!bOk) {
        return;
    }
    char* newPath = addToPath(convertQStringToChar(str),addToPath(homePath,convertQStringToChar(item->text())));
    cout<<"NEW: "<<newPath<<endl;
    if(rename(oldPath,newPath)){
        QMessageBox::warning(this, "Attention!","ERROR: error of move file!");
    }
    cout<<convertQStringToChar(ui->pathInput->text())<<endl;
    ui->listWidget->clear();
    showFiles(convertQStringToChar(ui->pathInput->text()),CATALOG_TYPE);
    showFiles(convertQStringToChar(ui->pathInput->text()),FILE_TYPE);
    return;
}


void MainWindow::on_homeButton_clicked()
{
    ui->listWidget->clear();
    showFiles(homePath,CATALOG_TYPE);
    showFiles(homePath,FILE_TYPE);
    return;
}

