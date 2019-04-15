#include "fs_folder.h"
#include "runtime_exception.h"

using namespace std;

FsFolder::FsFolder(string & name) {
    this->name = name;
}

string FsFolder::getFileContent() {
    throw RuntimeException("This operation is not supported by folder");
}

void FsFolder::add(FsComponent * component) {
    children.push_back(component);
}

void FsFolder::remove(FsComponent * component) {
    children.remove(component);
}

bool FsFolder::isComposite() {
    return true;
}

string FsFolder::getName() {
    return name;
}

string FsFolder::findFileAndGetContent(string &filePath) {
    auto delimiter_pos = filePath.find_first_of('/');
    if (delimiter_pos != 0 || filePath.length() <= 1) {
        return "";
    }
    auto next_delimiter_pos = filePath.find_first_of('/', 1);
    if (next_delimiter_pos == string::npos) {
        for (auto & cur : children) {
            if (!cur->isComposite()) {
                auto result = cur->findFileAndGetContent(filePath.erase(0, 1));
                if (!result.empty()) {
                    return result;
                }
            }
        }
        return "";
    }
    auto next_folder = filePath.substr(1, next_delimiter_pos - 1);
    for (auto & cur : children) {
        if (cur->isComposite() && cur->getName() == next_folder) {
            return cur->findFileAndGetContent(filePath.erase(0, next_delimiter_pos));
        }
    }
    return "";
}

FsFolder::~FsFolder() {
    for (auto & cur : children) {
        delete cur;
    }
}
