#include <init_params.h>
#include <socket.h>
#include <iostream>
#include "runtime_exception.h"
#include <app.h>
#include <handler.h>
#include <middleware.h>
#include <json_middleware.h>

class HandlerCommon : public Handler {
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string fd("<!doctype html><html><body><center><h1>TEST</h1></center><p>");

        fd += getContext()->getRequest()->getURI()->getUri();

        fd += "</p><table>";
        for (std::pair<std::string, std::string> some : getContext()->getRequest()->getHeaders()->getHeaders()) {
            fd += "<tr><td>";
            fd += some.first;
            fd += "</td><td>";
            fd += some.second;
            fd += "</td></tr>";
        }

        fd += "</table><hr/><hr/><table>";

        for (std::pair<std::string, std::string> some : getContext()->getRequest()->getURI()->getParams()) {
            fd += "<tr><td>";
            fd += some.first;
            fd += "</td><td>";
            fd += some.second;
            fd += "</td></tr>";
        }

        fd += "</table></body></html>";

        MessageBody fff = MessageBody(fd);

        //Response dd = Response(, 200, ddd,  fff);

        getContext()->getResponse()->setVersion(HTTP::Version::HTTP_1_1);
        getContext()->getResponse()->setStatus(200);
        getContext()->getResponse()->setHeaders(ddd);
        getContext()->getResponse()->setBody(fff);
    }
};

class HandlerMain : public Handler {
public:
    HandlerMain(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string fd("<!doctype html><html><body><center><h1>MAIN</h1></center><p>some text</p></body></html>");
        MessageBody fff = MessageBody(fd);

        Response dd = Response(HTTP::Version::HTTP_1_1, 200, ddd,  fff);

        getContext()->setResponse(dd);
    }
};


class HandlerJson : public Handler {
public:
    HandlerJson(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {


        Middleware * f = this->getContext()->getMiddlewareByNameID("json");

        if (f) {
            auto * j = (JsonMiddleware *) (void *) f;
            j->setEchoReply();
            j->fillResponse();
        }
    }
};

class HandlerContact : public Handler {
public:
    HandlerContact(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string fd("<!doctype html><html><body><center><h1>!Contact</h1></center><p>some text and other..</p></body></html>");
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
    HandlerContact * sdsf = new HandlerContact("/contact", HTTP::Method::GET);
    HandlerJson * nbv = new HandlerJson("/api", HTTP::Method::GET);
    website.addHandler(dada);
    website.addHandler(sdf);
    website.addHandler(sdsf);
    website.addHandler(nbv);
    website.addPermanentlyRedirect("/index", "/");
    website.addPermanentlyRedirect("/index.html", "/");
    website.addPermanentlyRedirect("/index.php", "/");
    website.addPermanentlyRedirect("/old", "/main");

    JsonMiddleware * json = new JsonMiddleware("json", nullptr, nullptr);
    website.addMiddleware(json);
    website.run();
    return 0;
}
