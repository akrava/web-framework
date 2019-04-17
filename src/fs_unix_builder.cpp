#ifdef __linux__

#include <dirent.h>
#include <limits.h>
#include "fs_unix_builder.h"
#include "runtime_exception.h"

using namespace std;

static string joinPath(string one, string & another);

FsFile *FsUnixBuilder::buildFile(string & filePath) {
    auto delimiter_pos = filePath.find_last_of('/');
    if (delimiter_pos == string::npos || delimiter_pos + 1 > filePath.length()) {
        throw RuntimeException("Couldn't build file, filePath is wrong");
    }
    auto fileName = filePath;
    fileName.erase(0, delimiter_pos + 1);
    return new FsFile(fileName, filePath);
}

FsFolder *FsUnixBuilder::buildFolder(string & folderPath, const char * folderName) {
    auto dir = opendir(folderPath.c_str());
    if(dir == nullptr) {
        throw RuntimeException("Couldn't open directory" + folderPath);
    }
    auto folder = new FsFolder(folderName);
    auto entity = readdir(dir);
    while (entity != nullptr) {
        string name = entity->d_name;
        string currentPath = joinPath(folderPath, name);
        if (entity->d_type == DT_DIR) {
            if (name[0] == '.') {
                entity = readdir(dir);
                continue;
            }
            folder->add(buildFolder(currentPath, entity->d_name));
            entity = readdir(dir);
            continue;
        }
        if (entity->d_type == DT_REG) {
            char buf[PATH_MAX + 1];
            realpath(currentPath.c_str(), buf);
            string filePath = buf;
            folder->add(buildFile(filePath));
        }
        entity = readdir(dir);
    }
    closedir(dir);
    root = folder;
    return folder;
}

string joinPath(string one, string & another) {
    if (one[one.length() - 1] != '/') {
        one.append("/");
    }
    return one + another;
}

#endif
