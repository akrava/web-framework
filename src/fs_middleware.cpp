#include "fs_middleware.h"
#include "fs_unix_builder.h"
#include "fs_windows_builder.h"

using namespace std;

FsMiddleware::FsMiddleware(const char *nameID, string & folderPath, string & mountPath) : Middleware(nameID) {
    creator.setBuilder(selectBuilder());
    if (mountPath.length() > 1) {
        if (mountPath[0] != '/') {
            mountPath.insert(0, "/");
        }
        if (mountPath[mountPath.length() - 1] == '/') {
            mountPath.erase(mountPath.length() - 1, 1);
        }
        this->mountPath = mountPath;
    }
    creator.makeFs(folderPath);
    folder = creator.getFs();
}

bool FsMiddleware::autoExec() {
    auto request_uri = request->getURI()->getPath();
    if (!mountPath.empty() && request_uri.find_first_of(mountPath) == 0) {
        request_uri.erase(0, mountPath.length());
    }
    value = folder->findFileAndGetContent(request_uri);
    return !value.empty();
}

void FsMiddleware::exec() {
    auto request_uri = request->getURI()->getPath();
    MessageBody body {value};
    response->setStatus(200);
    response->setBody(body);
    auto extension_pos = request_uri.find_last_of('.');
    string extension = request_uri;
    if (extension_pos != string::npos && extension_pos + 1 < request_uri.length()) {
        extension.erase(extension_pos + 1);
    } else {
        extension = "";
    }
    response->getHeaders()->add("Content-Type", HTTP::getMimeType(extension).c_str());
}

FsBuilder * FsMiddleware::selectBuilder() {
    #ifdef __linux__
        return new FsUnixBuilder();
    #elif defined(_WIN32) || defined(_WIN64)
        return new FsWindowsBuilder();
    #else
        return nullptr;
    #endif
}
