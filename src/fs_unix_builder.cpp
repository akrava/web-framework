#ifdef __linux__

#include <dirent.h>
#include <limits.h>
#include "fs_unix_builder.h"
#include "runtime_exception.h"

using namespace std;

FsFile *FsUnixBuilder::buildFile(string & filePath) {
    auto delimiter_pos = filePath.find_last_of('/');
    if (delimiter_pos == string::npos || delimiter_pos + 1 <= filePath.length()) {
        throw RuntimeException("Couldn't build file, filePath is wrong");
    }
    auto fileName = filePath;
    fileName.erase(0, delimiter_pos + 1);
    return new FsFile(fileName, filePath);
}

FsFolder *FsUnixBuilder::buildFolder(string & folderPath) {
    auto dir = opendir(folderPath.c_str());
    if(dir == nullptr) {
        throw RuntimeException("Couldn't open directory" + folderPath);
    }
    auto entity = readdir(dir);
    auto * folder = new FsFolder(entity->d_name);
    while (entity != nullptr) {
        string name = entity->d_name;
        if (entity->d_type == DT_DIR) {
            if (name[0] == '.') {
                entity = readdir(dir);
                continue;
            }
            folder->add(buildFolder(name));
            entity = readdir(dir);
            continue;
        }
        if (entity->d_type == DT_REG) {
            char buf[PATH_MAX + 1];
            realpath(entity->d_name, buf);
            string filePath = buf;
            folder->add(buildFile(filePath));
        }
        entity = readdir(dir);
    }
    closedir(dir);
    root = folder;
    return folder;
}


#endif
