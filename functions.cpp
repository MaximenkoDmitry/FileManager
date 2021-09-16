#include<functions.h>

using namespace std;

char* deleteNameFromPath(char* path){
    string Path(path);
    int i = Path.length(),count = 0;
    while(i>=0){
        if(Path[i] == '/'){
            Path.erase(Path.length()-count);
            if(Path == ""){
                Path = "/";
            }
            char* new_path = NULL;
            new_path = (char*)calloc(sizeof(char),Path.length());
            for(unsigned long i = 0;i<Path.length();i++){
                new_path[i] = Path[i];
            }
            return new_path;
        }
        count++;
        i--;
    }
    return path;
}

char* addToPath(char* path, char* name){
    string Path(path), Name(name);
    Path = Path+Name;
    char* result = NULL;
    result = (char*)calloc(sizeof(char),Path.length());
    for(unsigned long i = 0;i<Path.length();i++){
        result[i] = Path[i];
    }
    return result;
}

char* convertQStringToChar(QString qLine){
    char *line = convertStringToChar(qLine.toStdString());
    return line;
}

string convertCharToString(char* line){
    string convertLine(line);
    return convertLine;
}

char* convertStringToChar(string line){
    char* convertLine = NULL;
    convertLine = (char*)calloc(sizeof(char),line.length()+1);
    for(unsigned long i = 0;i<line.length();i++){
        convertLine[i] = line[i];
    }
    return convertLine;
}

char* getInformationAboutFile(char* path, char* name){
    struct stat info;
    stat(path, &info);
    //get Time
    struct tm * timeinfo;
    char* time = (char*)calloc(30,sizeof(char));
    stat(path, &info);
    time_t rawtime = info.st_ctim.tv_sec;
    timeinfo = localtime ( &rawtime );
    strftime (time,80,"%X %x.",timeinfo);

    //get type
    string type;
    if(S_ISREG(info.st_mode))
        type = "file";
    else{
        type = "catalog";
    }

    //getSize
    string size = to_string(info.st_size/1024);

    //get inode
    string inode = to_string(info.st_ino);

    //result
    string result = "Name: "+ convertCharToString(name) + "\n" +
                    "Type: " + type + "\n" +
                    "Size: " + size + " bytes\n" +
                    "Inode: " + inode + "\n" +
                    "Last modified time: " + time;
    return convertStringToChar(result);
}

bool checkFileType(struct stat* info) {
    return (S_ISREG(info->st_mode));
}

bool checkCatalogType(struct stat* info) {
    return (S_ISDIR(info->st_mode));
}
