#if defined(_WIN32) || defined(_WIN64)
#include "fs_windows_builder.h"

FsFile *FsWindowsBuilder::buildFile(std::string filePath) {
    return nullptr;
}

FsFolder *FsWindowsBuilder::buildFolder(std::string folderPath) {
    return nullptr;
}

#endif