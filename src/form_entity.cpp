#include "form_entity.h"

using namespace std;

FormEntity::FormEntity(string &value) {
    this->value = value;
    next = nullptr;
    isFile = false;
}

FormEntity::~FormEntity() {
    delete next;
    next = nullptr;
}

void FormEntity::setFileName(string &filename) {
    fileName = filename;
    isFile = true;
}

std::string FormEntity::getFileName() {
    return fileName;
}

void FormEntity::setContentType(string &contentType) {
    this->contentType = contentType;
    isFile = true;
}

std::string FormEntity::getContentType() {
    return contentType;
}

void FormEntity::setCharset(string charset) {
    this->charset = charset;
    isFile = true;
}

std::string FormEntity::getCharset() {
    return charset;
}

std::string FormEntity::getValue() {
    return value;
}

bool FormEntity::isFileContent() {
    return isFile;
}

void FormEntity::setNext(FormEntity *next) {
    if (this->next) {
        this->next->setNext(next);
    } else {
        this->next = next;
    }
}

FormEntity *FormEntity::getNext() {
    return next;
}

list<FormEntity *> FormEntity::getArrayValue() {
    auto res = list<FormEntity *>();
    res.push_back(this);
    auto cur = next;
    while (cur) {
        res.push_back(cur);
        cur = cur->next;
    }
    return  res;
}

void FormEntity::setValue(string & val) {
    value = val;
}
