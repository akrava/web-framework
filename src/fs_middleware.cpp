#include "fs_middleware.h"
#include "fs_unix_builder.h"
#include "fs_windows_builder.h"

using namespace std;

FsMiddleware::FsMiddleware(const char *nameID, const char * folderPath, const char * mountPath) : Middleware(nameID) {
    string mount = mountPath ? mountPath :  "/";
    creator.setBuilder(selectBuilder());
    if (mount.length() > 1) {
        if (mount[0] != '/') {
            mount.insert(0, "/");
        }
        if (mount[mount.length() - 1] == '/') {
            mount.erase(mount.length() - 1, 1);
        }
        this->mountPath = mount;
    }
    string f_path = folderPath;
    creator.makeFs(f_path);
    folder = creator.getFs();
}

bool FsMiddleware::autoExec() {
    auto request_uri = request->getURI()->getPath();
    if (!mountPath.empty() && request_uri.find(mountPath) == 0) {
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
        extension.erase(0, extension_pos + 1);
    } else {
        extension = "";
    }
    response->getHeaders()->add("Content-Type", HTTP::getMimeType(extension).c_str());
    response->finalize();
}

FsBuilder * FsMiddleware::selectBuilder() {
    #ifdef __linux__
        return new FsUnixBuilder();
    #elif defined(_WIN32)
        return new FsWindowsBuilder();
    #else
        return nullptr;
    #endif
}
