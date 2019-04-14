#include <app.h>
#include <runtime_exception.h>
#include <iostream>
#include <parser_http.h>
#include <default_response.h>
#include <socket_api.h>

using namespace std;

App::App(const char * ip, int port, bool isIPv6, const char *logFilePath)
        : network(ip, port, isIPv6), log(logFilePath)
{
    handlersRoutes = unordered_map<string, Handler *>();
    handler = nullptr;
    redirects = list<RedirectResponse>();
    middlewareList = vector<Middleware *>();
    network.setAPI(Network::createNewSocket());
}

App::App(InitParams & params) : network(params), log(params.getFilePath().c_str()) {
    handlersRoutes = unordered_map<string, Handler *>();
    handler = nullptr;
    redirects = list<RedirectResponse>();
    middlewareList = vector<Middleware *>();
    network.setAPI(Network::createNewSocket());
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
        if (handlersRoutes[handler->getRoute()]) {
            handlersRoutes[handler->getRoute()]->add(handler);
        } else {
            handlersRoutes[handler->getRoute()] = handler;
        }
    } else {
        if (this->handler) {
            this->handler->add(handler);
        } else {
            this->handler = handler;
        }
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
        bool executed = handler;
        if (handler) {
            handler->exec();
        }
        auto routedHandler = handlersRoutes[request->getURI()->getPath()];
        if (routedHandler && (routedHandler->getMethod() == request->getMethod()
            || routedHandler->getMethod() == HTTP::Method::ANY)) {
            context.getResponse()->setStatus(200);
            handlersRoutes[request->getURI()->getPath()]->exec();
            executed = true;
        } else if (routedHandler) {
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
        it.second->cleanNextHandlers();
    }
    if (handler) {
        handler->cleanNextHandlers();
    }
    for (auto * it : middlewareList) {
        delete it;
    }
}
