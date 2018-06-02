#include <init_params.h>
#include <socket.h>
#include <iostream>
#include "runtime_exception.h"
#include <app.h>
#include <handler.h>
#include <middleware.h>
#include <json_middleware.h>
#include <cookie_middleware.h>
#include <form_middleware.h>
#include <html_middleware.h>
#include <sqlite3.h>
#include <file_handler.h>
#include <db_manager.h>
#include <fstream>

class HandlerCommon : public Handler {
public:
    HandlerCommon(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {

        std::string headers_1("Content-Type: text/html; charset=utf-8");
        Headers header = Headers(headers_1);

        std::string body_html("<!doctype html><html><body><center><h1>TEST</h1></center><p>");

        body_html += getContext()->getRequest()->getURI()->getUri();

        body_html += "</p><table>";
        for (std::pair<std::string, std::string> some : getContext()->getRequest()->getHeaders()->getHeaders()) {
            body_html += "<tr><td>";
            body_html += some.first;
            body_html += "</td><td>";
            body_html += some.second;
            body_html += "</td></tr>";
        }

        body_html += "</table><hr/><hr/><table>";

        for (std::pair<std::string, std::string> some : getContext()->getRequest()->getURI()->getParams()) {
            body_html += "<tr><td>";
            body_html += some.first;
            body_html += "</td><td>";
            body_html += some.second;
            body_html += "</td></tr>";
        }

        body_html += "</table></body></html>";

        MessageBody message_body = MessageBody(body_html);

        getContext()->getResponse()->setVersion(HTTP::Version::HTTP_1_1);
        getContext()->getResponse()->setStatus(200);
        getContext()->getResponse()->setHeaders(header);
        getContext()->getResponse()->setBody(message_body);
    }
};

class HandlerMain : public Handler {
public:
    HandlerMain() : Handler(){}
    void exec() {
        std::string headrs_main("Content-Type: text/html; charset=utf-8");
        Headers headers_obj = Headers(headrs_main);

        std::ifstream in("../data/common.html", std::ifstream::in);
        std::string temp;
        if (in.is_open()) {
            std::stringstream str_stream;
            str_stream << in.rdbuf();
            temp = str_stream.str();
            in.close();
        }

        Middleware * middleware = this->getContext()->getMiddlewareByNameID("html");
        if (!middleware) return;

        auto *html = (HtmlMiddleware *) (void *) middleware;

        std::vector<std::vector<std::string>> res_tabl;

        char * data_temp[] = { (char *)(getContext()->getRequest()->getURI()->getUri()).c_str() };
        getContext()->getDB()->execQuery("SELECT * FROM pages WHERE uri=?",res_tabl, data_temp, 1);

        std::string res_val_title;
        if (!(res_tabl.empty() || res_tabl[0].empty())) {
            res_val_title = res_tabl[0][2];
        }


        html->getContext()->insert({"title",  res_val_title});
        html->setView(temp);
        getContext()->getResponse()->setVersion(HTTP::Version::HTTP_1_1);
        getContext()->getResponse()->setStatus(200);
        getContext()->getResponse()->setHeaders(headers_obj);
        html->exec();
    }
};


class HandlerJson : public Handler {
public:
    HandlerJson(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {

        Middleware * middleware = this->getContext()->getMiddlewareByNameID("json");

        if (!middleware) return;

        auto * json = (JsonMiddleware *) (void *) middleware;
        json->setEchoReply();
        json->fillResponse();

    }
};


class HandlerCookie : public Handler {
public:
    HandlerCookie(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {

        Middleware * middleware = this->getContext()->getMiddlewareByNameID("cookie");

        if (!middleware) return;

        auto * cookie = (CookieMiddleware *) (void *) middleware;

        int val = 0;
        auto iterator = cookie->getMap()->find("Val");
        if (iterator != cookie->getMap()->end()) {
            val = std::stoi(iterator->second);
        }
        val++;

        CookieEntity some = CookieEntity(std::to_string(val).c_str());
        cookie->addCooike("Val", some);
        cookie->insertInResponse();

        std::string str = getContext()->getResponse()->getBody()->getBody();
        size_t  pos = str.find("</body></html>");
        std::string inject = "<p>Val: ";
        inject += std::to_string(val);
        inject += "</p>";
        str.insert(pos, inject);
        MessageBody message(str);
        getContext()->getResponse()->setBody(message);

    }
};

class HandlerForm : public Handler {
public:
    HandlerForm(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {

        Middleware * middleware = this->getContext()->getMiddlewareByNameID("form");
        if (!middleware) return;

        auto * form = (FormMiddleware *) (void *) middleware;

        std::string str = getContext()->getResponse()->getBody()->getBody();
        size_t  pos = str.find("</body></html>");
        std::string inject = "<hr/><hr/><p>DATA FROM FORM: </p> <table>";

        for (std::pair<std::string, std::string> some : *form->getMap()) {
            inject += "<tr><td>";
            inject += some.first;
            inject += "</td><td>";
            inject += some.second;
            inject += "</td></tr>";
        }

        inject += "</table>";
        str.insert(pos, inject);
        MessageBody body_m(str);
        getContext()->getResponse()->setBody(body_m);

    }
};

class HandlerContact : public Handler {
public:
    HandlerContact(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        std::string fdg("Content-Type: text/html; charset=utf-8");

        Headers ddd = Headers(fdg);
        std::string body("<!doctype html><html><body><center><h1>!Contact</h1></center><p>some text and other..</p></body></html>");
        MessageBody message = MessageBody(body);

        Response response_obj = Response(HTTP::Version::HTTP_1_1, 200, ddd,  message);

        getContext()->setResponse(response_obj);
    }
};

class HandlerHtml : public Handler {
public:
    HandlerHtml(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        Middleware * middleware = this->getContext()->getMiddlewareByNameID("html");
        if (!middleware) return;

        auto *html = (HtmlMiddleware *) (void *) middleware;
        html->getContext()->insert({"names", mstch::array{
                mstch::map{{"name", std::string{"Chris"}}},
                mstch::map{{"name", std::string{"Mark"}}},
                mstch::map{{"name", std::string{"Scott"}}}
        }});
        html->getContext()->insert({"test", mstch::array{
                mstch::map{{"some", std::string{"first line"}}},
                mstch::map{{"some", std::string{"second line"}}},
                mstch::map{{"some", std::string{"third line"}}}
        }});
        std::string view{"{{#names}}<p>Hi <b>{{name}}</b>!</p></br>{{/names}}{{#test}}<h5>{{some}}</h5></br>{{/test}}"};
        html->setView(view);
        html->exec();

    }
};


static int callback(void *data, int argc, char **argv, char **azColName){
    int i;

    HtmlMiddleware * html = (HtmlMiddleware *)data;

    std::string price;
    std::string reciver;
    std::string sender;
    std::string num;

    for (i = 0; i<argc; i++){
        if (!strcmp(azColName[i], "price")) {
            price = argv[i];
        } else if (!strcmp(azColName[i], "reciver")) {
            reciver = argv[i];
        } else if (!strcmp(azColName[i], "sender")) {
            sender = argv[i];
        } else if (!strcmp(azColName[i], "num")) {
            num = argv[i];
        }
    }

    html->getContext()->insert({"receipts", mstch::array{
            mstch::map{{"info", num + " " + reciver + " " + sender + " " + price}}
    }});

    return 0;
}

class HandlerTrack : public Handler {
public:
    HandlerTrack(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        Middleware *middleware = this->getContext()->getMiddlewareByNameID("html");

        if (!middleware) return;

        auto *html = (HtmlMiddleware *) (void *) middleware;


        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        char *sql;


        /* Open database */
        rc = sqlite3_open("../../db/db_file", &db);

        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        } else {
            fprintf(stderr, "Opened database successfully\n");
        }

        /* Create SQL statement */
        sql = "SELECT * FROM RECEIPT";

        rc = sqlite3_exec(db, sql, callback, (void*)html, &zErrMsg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
        sqlite3_close(db);

        std::string view{"{{#receipts}}<p>{{info}}</p></br>{{/receipts}}"};
        html->setView(view);
        html->exec();

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

    HandlerCommon * dada = new HandlerCommon("/main", HTTP::Method::GET);
    HandlerMain * sdf = new HandlerMain();
    HandlerContact * sdsf = new HandlerContact("/contact", HTTP::Method::GET);
    HandlerJson * nbv = new HandlerJson("/api", HTTP::Method::GET);
    HandlerCookie * cookies = new HandlerCookie("/cookie", HTTP::Method::GET);
    HandlerForm * forms_h = new HandlerForm("/post", HTTP::Method::GET);
    HandlerHtml * html_h = new HandlerHtml("/html", HTTP::Method::GET);
    HandlerTrack * track = new HandlerTrack("/db", HTTP::Method::GET);
    FileHandler * css = new FileHandler("/common.css", "../data/common.css", "text/css", false);
    FileHandler * img = new FileHandler("/logo.jpg", "../data/logo.jpg", "image/jpeg", true);

    website.addHandler(dada);
    website.addHandler(sdf);
    website.addHandler(sdsf);
    website.addHandler(nbv);
    website.addHandler(cookies);
    website.addHandler(forms_h);
    website.addHandler(html_h);
    website.addHandler(track);
    website.addHandler(css);
    website.addHandler(img);

    website.addPermanentlyRedirect("/index", "/");
    website.addPermanentlyRedirect("/index.html", "/");
    website.addPermanentlyRedirect("/index.php", "/");
    website.addPermanentlyRedirect("/old", "/main");

    JsonMiddleware * json = new JsonMiddleware("json", nullptr, nullptr);
    CookieMiddleware * cookie = new CookieMiddleware("cookie", nullptr, nullptr);
    FormMiddleware * form = new FormMiddleware("form", nullptr, nullptr);
    HtmlMiddleware * html = new HtmlMiddleware("html", nullptr, nullptr);

    website.addMiddleware(json);
    website.addMiddleware(cookie);
    website.addMiddleware(form);
    website.addMiddleware(html);

    website.run();
    return 0;
}
