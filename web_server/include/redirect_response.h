#pragma once

#include "response.h"

/**
 *
 */
class RedirectResponse : public Response {
    int redirectCode;
    std::string redirectUri;
    std::string targetUri;
public:
    /**
     *
     * @param redirectUri
     * @param targetUri
     */
    RedirectResponse(const char * redirectUri, const char * targetUri);

    /**
     *
     */
    void setPermanent();

    /**
     *
     */
    void setTemporary();

    /**
     *
     * @param code
     */
    void setRedirectCode(int code);

    /**
     *
     * @return
     */
    std::string  getRedirectUri();
};

