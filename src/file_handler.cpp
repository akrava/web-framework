#include <file_handler.h>
#include <fstream>
#include <sstream>
#include <iostream>

#define __MAX_SIZE_CACHED 5120

using namespace std;

FileHandler::FileHandler(const char * route, const char * filePath, const char * mimeType, bool isBinary)
                            : Handler (route, HTTP::Method::GET)
{
    this->filePath = filePath;
    this->mimeType = mimeType;
    binary = isBinary;
    ifstream in(filePath, isBinary ? ifstream::ate | ifstream::binary : ifstream::ate);
    long size =  in.tellg();
    if (size > 0 && size < __MAX_SIZE_CACHED) {
        if (in.is_open()) {
            stringstream str_stream;
            str_stream << in.rdbuf();
            cache = str_stream.str();
            in.close();
        }
    }
}

void FileHandler::exec() {
    MessageBody body;
    if (!cache.empty()) {
        body = MessageBody(cache);
    } else {
        ifstream in(filePath, binary ? ifstream::in | ifstream::binary : ifstream::in);
        string temp;
        if (in.is_open()) {
            stringstream str_stream;
            str_stream << in.rdbuf();
            temp = str_stream.str();
            body = MessageBody(temp);
            in.close();
        } else {
            getContext()->getResponse()->setStatus(404);
            string err_s = "<html><head>404 Not Found</head><body>"
                           "<h1>404 Not found</h1></body></html>";
            MessageBody err = MessageBody(err_s);
            getContext()->getResponse()->setBody(err);
            return;
        }
    }
    getContext()->getResponse()->setStatus(200);
    getContext()->getResponse()->setBody(body);
    getContext()->getResponse()->getHeaders()->add("Content-Type", mimeType.c_str());
}

bool FileHandler::loadFile(const char *filePath, string & data) {
    ifstream in(filePath, ifstream::in);
    cout << filePath << endl;
    if (in.is_open()) {
        stringstream str_stream;
        str_stream << in.rdbuf();
        data = str_stream.str();
        in.close();
        return true;
    }
    return false;
}
