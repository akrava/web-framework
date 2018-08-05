#pragma once

#include "response.h"
#include <unordered_map>
#include "request.h"

/**
 * @brief class wrapper for middleware
 *
 * Middleware have got current request and response objects and also map for
 *      key-value pairs. Method exec and autoExec can use request and response
 *      objects to perform actions.
 */
class Middleware {
    std::string nameID;
protected:
    Request * request;
    Response * response;
    std::unordered_map<std::string, std::string> * map;
public:

    /**
     * Create empty middleware, where Request and Response objects are null
     *
     * @param nameID
     *      name id as string
     */
    Middleware(const char * nameID);


    virtual ~Middleware();

    /**
     * set request and response objects into Middleware
     *
     * @param request
     *      request object
     * @param response
     *      response object
     */
    void setContent(Request * request, Response * response);

    /**
     * Add value to map by key. If key exists, it should be overwritten
     *
     * @param key
     *      key as string
     * @param value
     *      value as string
     */
    void addValueToMap(const char * key, const char * value);

    /**
     * get value from map by key
     *
     * @param key
     *      needed key
     * @param value
     *      out param, if value exists should be written, do nothing otherwise
     * @return
     *      true if key exists in map, false otherwise
     */
    bool getValueFromMap(const char * key, std::string & value);

    /**
     * get map of key-value pairs
     *
     * @return
     *      map of key-value pairs
     */
    std::unordered_map<std::string, std::string> * getMap();

    /**
     * Check if current request allow do exec method
     *
     * @return
     *      true, if need do exec with current request, false otherwise
     */
    virtual bool autoExec() = 0;

    /**
     * perform operation with request and response objects
     */
    virtual void exec() = 0;

    /**
     * get name id of middleware
     *
     * @return
     *      name id
     */
    std::string getNameID();

    /**
     * delete all data, saved in internal values, set to default
     */
    virtual void clear();
};
