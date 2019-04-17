#include <fs_unix_builder.h>
#include "fs_creator.h"
#include "runtime_exception.h"

using namespace std;

FsCreator::~FsCreator() {
    delete builder;
}

void FsCreator::setBuilder(FsBuilder * builder) {
    delete this->builder;
    this->builder = builder;
}

void FsCreator::makeFs(string & folderPath) {
    if (!builder) {
        throw RuntimeException("Builder wasn't set");
    }
    builder->reset();
    builder->buildFolder(folderPath, "");
}

void FsCreator::makeFs(FsBuilder *builder, string & folderPath) {
    setBuilder(builder);
    makeFs(folderPath);
}

FsComponent *FsCreator::getFs() {
    if (!builder) {
        throw RuntimeException("Builder wasn't set");
    }
    return builder->getResult();
}
