#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<QString>
#include<string>
#include <sys/stat.h>
#include <sys/types.h>
#include<dirent.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

char* deleteNameFromPath(char* path);

char* addToPath(char* path, char* name);

char* convertQStringToChar(QString qLine);

string convertCharToString(char* line);

char* convertStringToChar(string line);

char* getInformationAboutFile(char* path, char* name);

bool checkFileType(struct stat* info);

bool checkCatalogType(struct stat* info);

#endif // FUNCTIONS_H
