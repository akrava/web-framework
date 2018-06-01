#pragma once
#include "response.h"

class RedirectResponse : public Response {
    int redirectCode;
    std::string redirectUri;
    std::string targetUri;
public:
    RedirectResponse(const char * redirectUri, const char * targetUri);
    void setPermanent();
    void setTemporary();
    void setRedirectCode(int code);
    std::string  getRedirectUri();
};

