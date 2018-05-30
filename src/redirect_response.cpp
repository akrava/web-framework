#include "redirect_response.h"
#include <string>
#include <redirect_response.h>

RedirectResponse::RedirectResponse(const char * redirectUri, const char * targetUri) {
    this->redirectUri = redirectUri;
    this->targetUri = targetUri;
    redirectCode = 404;
}

void RedirectResponse::setPermanent() {
    setRedirectCode(301);
}

void RedirectResponse::setTemporary() {
    setRedirectCode(302);
}

void RedirectResponse::setRedirectCode(int code) {
    redirectCode = code;
    setStatus(code);
    setVersion(HTTP::Version::HTTP_1_1);
    this->getHeaders()->add("Location", targetUri.c_str());
}

std::string RedirectResponse::getRedirectUri() {
    return redirectUri;
}
