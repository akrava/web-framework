#pragma once

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include "response.h"
#include "handler.h"
#include "network.h"
#include "redirect_response.h"
#include "log_manager.h"
#include "middleware.h"
#include "context.h"

/**
 * @brief The main class of the framework. Each object of this class
 *      is an independent web-application, which could be configured
 *      by handlers, middleware etc.
 *
 * This class implements web-application, which is running on given
 *      and port. It supports IPv6 and can capture log in the file
 *      if given. Use Handlers, Middleware and set Redirects to
 *      adjust it.
 */
class App {
    // Socket socket;
    Network network;
    std::unordered_map<std::string, Handler *> handlersRoutes;
    Handler * handler;
    std::list<RedirectResponse> redirects;
    std::vector<Middleware *> middlewareList;
    Context context;
    LogManager log;
public:
    /**
     * Create a new web application, which is only adjusted to ip address. You can init this
     *      and add some handlers etc. to run this
     *
     * @param ip
     *      text representation of ip address, like 127.0.0.1 or 0:0:0:0:0:0:0:1 (if IPv6)
     * @param port
     *      port in range [0, 65535]
     * @param isIPv6
     *      set true, if param ip is version 6
     * @param logFilePath
     *      if you want to create log file, give a file path, or null otherwise
     */
    explicit App(const char * ip = "127.0.0.1", int port = 80, bool isIPv6 = false, const char * logFilePath = nullptr);

    /**
     * Create a new web application, by command line arguments using InitParams object
     *
     * @param params
     *      Give an object params, which was created by InitParams class from command line arguments
     */
    explicit App(InitParams & params);

    /**
     * Destructor delete all added handlers and all middleware
     */
    ~App();

    /**
     * Use this function to open socket for listening on declared ip address and port.
     *      After creating on object you should use this function to startup web-socket
     *
     * @return
     *      true if ip address and port were valid and available, false - otherwise,
     *      please, use another address to continue
     */
    bool init();

    /**
     * To configure your application create and add some handlers
     *
     * @param handler
     *      object of class Handler (could be inherited) with overridden function
     *      exec
     */
    void addHandler(Handler * handler);

    /**
     * Add redirection, which is meant to last forever. The original URL should not be
     *      used anymore and that the new one is preferred. Search engine robots trigger
     *      an update of the associated URL for the resource in their indexes. (HTTP code 301)
     *
     * @param uri
     *      original uri path, which is deprecated (outdated)
     * @param target
     *      new uri address of mentioned page
     */
    void addPermanentlyRedirect(const char * uri, const char * target);

    /**
     * Temporary redirect can be used, if for some time the requested resource cannot be accessed
     *      from its canonical location, but it can be accessed from another place. Search engine
     *      robots don't memorize the new, temporary link. Temporary redirection are also used
     *      when creating, updating and deleting resources to present temporary progress pages.
     *
     * @param uri
     *      original uri path, which is deprecated (outdated)
     * @param target
     *      new uri address of mentioned page
     */
    void addTemporaryRedirect(const char * uri, const char * target);

    /**
     * To adjust any redirection using status code. For example, 304 (Not Modified) redirects
     *      a page to the locally cached copy, and 300 (Multiple Choice) is a manual redirection:
     *      the body, presented by the browser as a Web page, lists the possible redirection and
     *      the user clicks on one to select it.
     *
     * @param uri
     *      original uri path, which is deprecated (outdated)
     * @param target
     *      new uri address of mentioned page
     * @param code
     *      HTTP code redirection status of response
     */
    void addRedirect(const char * uri, const char * target, int code);

    /**
     * Add object of class Middleware, which has got overridden function exec
     *      to do given operations on every request. All handlers could access
     *      to any middleware and perform adjusted actions.
     *
     * @param middleware
     *      object of class Middleware (could be inherited)
     */
    void addMiddleware(Middleware * middleware);

    /**
     * Start listening for request. This method startup the system, where on every request
     *      from clients all added handlers and middleware create a response and send it
     *      to client.
     */
    bool run();
};
