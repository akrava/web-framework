#include <sstream>
#include <fstream>
#include "fs_file.h"
#include "runtime_exception.h"

using namespace std;

FsFile::FsFile(string & name, string & fullPath) {
    this->name = name;
    this->fullPath = fullPath;
}

string FsFile::getFileContent() {
    ifstream in(fullPath, ifstream::in | ifstream::binary);
    if (in.is_open()) {
        stringstream str_stream;
        str_stream << in.rdbuf();
        auto data = str_stream.str();
        in.close();
        return data;
    }
    throw RuntimeException("Couldn't open file for reading");
}

void FsFile::add(FsComponent * component) {
    throw RuntimeException("This operation is not supported by file");
}

void FsFile::remove(FsComponent * component) {
    throw RuntimeException("This operation is not supported by file");
}

bool FsFile::isComposite() {
    return false;
}

string FsFile::getName() {
    return name;
}

string FsFile::findFileAndGetContent(string &filePath) {
    if (filePath != name) {
        return "";
    }
    return getFileContent();
}
