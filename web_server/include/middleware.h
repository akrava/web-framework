#pragma once

#include <response.h>
#include <unordered_map>
#include <request.h>

/**
 *
 */
class Middleware {
    std::string nameID;
protected:
    Request * request;
    Response * response;
    std::unordered_map<std::string, std::string> * map;
public:

    /**
     *
     * @param nameID
     * @param request
     * @param response
     */
    Middleware(const char * nameID, Request * request, Response * response);


    virtual ~Middleware();

    /**
     *
     * @param request
     * @param response
     */
    void setContent(Request * request, Response * response);

    /**
     *
     * @param key
     * @param value
     */
    void addValueToMap(const char * key, const char * value);

    /**
     *
     * @param key
     * @param value
     * @return
     */
    bool getValueFromMap(const char * key, std::string & value);

    /**
     *
     * @return
     */
    std::unordered_map<std::string, std::string> * getMap();

    /**
     *
     * @return
     */
    virtual bool autoExec() = 0;

    /**
     *
     */
    virtual void exec() = 0;

    /**
     *
     * @return
     */
    std::string getNameID();
};
