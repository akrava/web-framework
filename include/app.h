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

class App {
    Socket socket;
    std::unordered_map<std::string, Handler *> handlersRoutes;
    std::list<Handler *> handlersChain;
    Context context;
    logManager log;
public:
    App(std::string & ip, int port = 80, bool isIPv6 = false, const char * logFilePath = nullptr);
    App(InitParams params);
    bool init();
    void addHandler(Handler * handler);
    //void addHandler(std::list<Handler> & handlerList);
    //void addHandler(std::vector<Handler> & handlerList);
    void run();
    void close();
};
