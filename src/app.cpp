#include <app.h>
#include <runtime_exception.h>
#include <iostream>
#include <parser_http.h>
#include <request.h>
#include <response.h>

App::App(std::string & ip, int port, bool isIPv6, const char *logFilePath) : socket(ip, port, isIPv6) {
    redirects = std::list<RedirectResponse>();
    // todo: logging in file- / console- stream
}

App::App(InitParams params) : socket(params) {
    redirects = std::list<RedirectResponse>();
    // todo: logging init also
    params.getFilePath();
}

void App::addPermanentlyRedirect(const char * uri, const char * target) {
    RedirectResponse res = RedirectResponse(uri, target);
    res.setPermanent();
    redirects.push_back(res);
}

void App::addTemporaryRedirect(const char * uri, const char * target) {
    RedirectResponse res = RedirectResponse(uri, target);
    res.setTemporary();
    redirects.push_back(res);
}

void App::addRedirect(const char * uri, const char * target, int code) {
    RedirectResponse res = RedirectResponse(uri, target);
    res.setRedirectCode(code);
    redirects.push_back(res);
}


bool App::init() {
    try {
        socket.init();
    } catch (RuntimeException & err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    return true;
}

void App::addHandler(Handler * handler) {
    handler->setContext(&context);
    if (handler->isRouted()) {
        handlersRoutes.insert({handler->getRoute(), handler});
    } else {
        handlersChain.push_back(handler);
    }
}

//
//void App::addHandler(std::list<Handler> handlerList) {
//
//}
//
//void App::addHandler(std::vector<Handler> handlerList) {
//
//}

void App::run() {
    std::string d;
    while (true) {

        try {
            d = socket.getData();

            Request request = ParserHTTP::getRequestFromStr(d);
            context.setRequest(request);

            for (auto & cur : redirects) {
                if (cur.getRedirectUri() == request.getURI().getUri()) {
                    std::string ttt = ParserHTTP::getStrFromResponse(cur);
                    socket.reciveData(ttt);
                    continue;
                }
            }


            for (auto * cur : handlersChain) {
                cur->exec();
            }

            if (handlersRoutes.find(request.getURI().getUri()) != handlersRoutes.end()) {
                handlersRoutes[request.getURI().getUri()]->exec();
            }

//            std::string ex = "<!doctype html><html><body><center><h1>TEST</h1></center><h2>/</h2><h3>? - ?</h3><p>.,.... </p></body></html>";
//
//            std::string q1("HTTP/1.1 200 OK\r\n");
//            socket.reciveData(q1);
//
//            std::string q2("Content-length: 109\r\n");
//            socket.reciveData(q2);
//
//            std::string q3("Content-Type: text/html; charset=utf-8\r\n");
//            socket.reciveData(q3);
//
//            std::string q4("Connection: close\r\n\r\n");
//            socket.reciveData(q4);
            auto dd = context.getResponse();
            std::string rsponse = ParserHTTP::getStrFromResponse(dd);

            socket.reciveData(rsponse);

        } catch (RuntimeException & err) {
            std::cerr << err.what() << std::endl;
            break;
        }
        if (d== "exit") break;
    }


}

void App::close() {

}
