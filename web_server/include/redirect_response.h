#pragma once

#include <response.h>

/**
 * @brief Response class which is intended to make http redirects
 *
 * Inherited class RedirectResponse from Response for easiest adjusting redirects
 */
class RedirectResponse : public Response {
    int redirectCode;
    std::string redirectUri;
    std::string targetUri;
public:
    /**
     * Create RedirectResponse object with redirect code 404 (you should use method to set
     *      required redirect code)
     *
     * @param redirectUri
     *      input uri, which must be redirected
     * @param targetUri
     *      destination redirect uri
     */
    RedirectResponse(const char * redirectUri, const char * targetUri);

    /**
     * set permanent http redirect
     */
    void setPermanent();

    /**
     * set temporary http redirect
     */
    void setTemporary();

    /**
     * set redirect code status
     *
     * @param code
     *      http redirect code status
     */
    void setRedirectCode(int code);

    /**
     * get target uri from redirect response
     *
     * @return
     *      destination redirect uri
     */
    std::string getRedirectUri();
};
