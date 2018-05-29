#include <init_params.h>
#include <socket.h>
#include <iostream>
#include "runtime_exception.h"
#include <app.h>
#include <handler.h>

class HandlerCommon : public Handler {
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string fd("<!doctype html><html><body><center><h1>TEST</h1></center><h2>/</h2><h3>? - ?</h3><p>.,.... </p></body></html>");
        MessageBody fff = MessageBody(fd);

        Response dd = Response(HTTP::Version::HTTP_1_1, 200, ddd,  fff);

        getContext()->setResponse(dd);
    }
};

class HandlerMain : public Handler {
public:
    HandlerMain(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string fd("<!doctype html><html><body><center><h1>MAIN</h1></center><p></p></body></html>");
        MessageBody fff = MessageBody(fd);

        Response dd = Response(HTTP::Version::HTTP_1_1, 200, ddd,  fff);

        getContext()->setResponse(dd);
    }
};

int main (int argc, char ** argv) {
    InitParams cur;
    try {
        cur = InitParams(argc, argv);
    } catch (RuntimeException & err) {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    App website = App(cur);
    website.init();
    //std::cout << "YES!";

    HandlerCommon * dada = new HandlerCommon();
    HandlerMain * sdf = new HandlerMain("/main", HTTP::Method::GET);
    website.addHandler(dada);
    website.addHandler(sdf);

    website.run();
    return 0;
}
