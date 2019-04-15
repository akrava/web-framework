#include "fs_middleware.h"

using namespace std;

FsMiddleware::FsMiddleware(const char *nameID, string & folderPath, string & mountPath) : Middleware(nameID) {

}

bool FsMiddleware::autoExec() {
    return false;
}

void FsMiddleware::exec() {

}
