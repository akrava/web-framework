#pragma once
#include <string>
#include <list>
#include <init_params.h>
#include <vector>
#include <context.h>
#include <log_manager.h>
#include <socket.h>
#include <handler.h>
#include <unordered_map>
#include "redirect_response.h"

class App {
    Socket socket;
    std::unordered_map<std::string, Handler *> handlersRoutes;
    std::list<Handler *> handlersChain;
    std::list<RedirectResponse> redirects;
    Context context;
    logManager log;
public:
    App(std::string & ip, int port = 80, bool isIPv6 = false, const char * logFilePath = nullptr);
    App(InitParams params);
    bool init();
    void addHandler(Handler * handler);
    void addPermanentlyRedirect(const char * uri, const char * target);
    void addTemporaryRedirect(const char * uri, const char * target);
    void addRedirect(const char * uri, const char * target, int code);
    //void addHandler(std::list<Handler> & handlerList);
    //void addHandler(std::vector<Handler> & handlerList);
    void run();
    void close();
};
