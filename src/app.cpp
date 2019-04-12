#include <app.h>
#include <runtime_exception.h>
#include <iostream>
#include <parser_http.h>
#include <default_response.h>
#include "socket_unix_api.h"
#include "socket_windows_api.h"

using namespace std;

static SocketAPI * getNetwork();

App::App(const char * ip, int port, bool isIPv6, const char *logFilePath)
        : network(ip, port, isIPv6), log(logFilePath)
{
    handlersRoutes = unordered_map<string, Handler *>();
    handlersChain = list<Handler *>();
    redirects = list<RedirectResponse>();
    middlewareList = vector<Middleware *>();
    network.setAPI(getNetwork());
}

App::App(InitParams & params) : network(params), log(params.getFilePath().c_str()) {
    handlersRoutes = unordered_map<string, Handler *>();
    handlersChain = list<Handler *>();
    redirects = list<RedirectResponse>();
    middlewareList = vector<Middleware *>();
    network.setAPI(getNetwork());
}

bool App::init() {
    try {
        network.init();
    } catch (RuntimeException & err) {
        cerr << err.what() << endl;
        log << err.what();
        return false;
    }
    log << "Launched successfully on " + network.toString();
    return true;
}

void App::addHandler(Handler * handler) {
    log << "Added handler";
    handler->setContext(&context);
    if (handler->isRouted()) {
        handlersRoutes.insert({handler->getRoute(), handler});
    } else {
        handlersChain.push_back(handler);
    }
}

void App::addMiddleware(Middleware * middleware) {
    middleware->setContent(context.getRequest(), context.getResponse());
    middlewareList.push_back(middleware);
    log << "Added middleware " + middleware->getNameID();
}

void App::addPermanentlyRedirect(const char * uri, const char * target) {
    redirects.emplace_back(uri, target);
    redirects.back().setPermanent();
    log << "Added permanently redirect: " + string{uri} + " to " + target;
}

void App::addTemporaryRedirect(const char * uri, const char * target) {
    RedirectResponse res = RedirectResponse(uri, target);
    res.setTemporary();
    redirects.push_back(res);
    log << "Added temporary redirect: " + string{uri} + " to " + target;
}

void App::addRedirect(const char * uri, const char * target, int code) {
    RedirectResponse res = RedirectResponse(uri, target);
    res.setRedirectCode(code);
    redirects.push_back(res);
    log << "Added " + to_string(code) + " redirect: " + string{uri} + " to " + target;
}

bool App::run() {
    context.setMiddlewareList(&middlewareList);
    log << "Running application";
    bool run = true;
    bool error = false;
    while (run) {
        string request_str;
        try {
            request_str = network.readData();
        } catch (RuntimeException & err) {
            cerr << err.what() << endl;
            log << err.what();
            error = true;
            break;
        }
        Request * request = ParserHTTP::getRequestFromStr(request_str);
        log << "Got request: " + request->getURI()->getRawData();
        context.setRequest(request);
        for (auto & cur : redirects) {
            if (cur.getRedirectUri() == request->getURI()->getPath()) {
                string data = ParserHTTP::getStrFromResponse(cur);
                network.receiveData(data);
                continue;
            }
        }
        for (auto * cur : middlewareList) {
            cur->clear();
            cur->setContent(context.getRequest(), context.getResponse());
            if (cur->autoExec()) cur->exec();
        }
        bool executed = !handlersChain.empty();
        for (auto * cur : handlersChain) {
            cur->exec();
        }
        auto handler_route = handlersRoutes.find(request->getURI()->getPath());
        if (handler_route != handlersRoutes.end()
            && (handler_route->second->getMethod() == HTTP::Method::ANY
                || handler_route->second->getMethod() == request->getMethod()))
        {
            context.getResponse()->setStatus(200);
            handlersRoutes[request->getURI()->getPath()]->exec();
            executed = true;
        } else if ((handler_route != handlersRoutes.end())) {
            auto * res = new DefaultResponse{405};
            context.setResponse(res);
        }
        if (!executed) {
            auto * res = new DefaultResponse{-1, "Add some handlers..."};
            context.setResponse(res);
        }
        auto * response = context.getResponse();
        string response_str = ParserHTTP::getStrFromResponse(*response);
        try {
            network.receiveData(response_str);
        } catch (RuntimeException & err) {
            cerr << err.what() << endl;
            log << err.what();
            error = true;
            break;
        }
        if (context.isClosed()) run = false;
    }
    log << "Got close event. Closing application";
    cout << "Host shutdown" << endl;
    return !error;
}

App::~App() {
    log << "App destroyed";
    for (auto & it : handlersRoutes) {
        delete it.second;
    }
    for (auto * it : handlersChain) {
        delete it;
    }
    for (auto * it : middlewareList) {
        delete it;
    }
}

static SocketAPI * getNetwork() {
    #ifdef __linux__
        return new SocketUnixAPI();
    #elif _WIN32
        return new SocketWindowsAPI();
    #else
        return null;
    #endif
}