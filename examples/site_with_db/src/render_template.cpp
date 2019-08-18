#include <render_template.h>
#include <akrava/web-server/html_middleware.h>
#include <database_middleware.h>
#include <config.h>
#include <akrava/web-server/auth_middleware.h>
#include <user.h>

using namespace std;

void HandlerRenderTemplate::exec() {
    Request * request = getContext()->getRequest();
    Response * response = getContext()->getResponse();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    AuthMiddleware * auth = (AuthMiddleware *) getContext()->getMiddlewareByNameID("auth");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    
    vector<vector<string>> result_pages_nav_bar;
    if (!db->execQuery(
            "SELECT name, link FROM pages WHERE type = 'nav_bar'",
            result_pages_nav_bar,
            nullptr,
            0)
    ) return;
    mstch::array array_pages_nav_bar;
    for (auto & it :  result_pages_nav_bar) {
        if (it.size() != 2) continue;
        mstch::map current;
        current.insert({"name", it[0]});
        current.insert({"link", it[1]});
        array_pages_nav_bar.push_back(current);
    }
    html->getContext()->insert({"pages_nav_bar", array_pages_nav_bar});
    
    vector<vector<string>> result_pages_menu;
    if (!db->execQuery(
            "SELECT name, link FROM pages WHERE type = 'menu'",
            result_pages_menu,
            nullptr,
            0)
    ) return;
    mstch::array array_pages_menu;
    for (auto & it :  result_pages_menu) {
        if (it.size() != 2) continue;
        mstch::map current;
        current.insert({"name", it[0]});
        current.insert({"link", it[1]});
        array_pages_menu.push_back(current);
    }
    html->getContext()->insert({"pages_menu", array_pages_menu});
    
    vector<vector<string>> result_uri_receipt;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = 'uri_receipt'",
            result_uri_receipt,
            nullptr,
            0)
    ) return;
    if (result_uri_receipt.size() != 1 || result_uri_receipt[0].size() != 1) return;
    html->getContext()->insert({"uri_receipt", string{result_uri_receipt[0][0]}});
    
    if (request->getURI()->getPath().empty()) return;
    string uri = request->getURI()->getPath();
    const char * data[] = { uri.c_str() };
    vector<vector<string>> result_title;
    if (!db->execQuery(
            "SELECT title FROM pages WHERE link = ?",
            result_title,
            (char **) data,
            1)
    ) return;

    string prev = response->getBody()->getBody();
    string title;
    string body;
    if (result_title.size() == 1 && result_title[0].size() == 1)  {
        title = string{result_title[0][0]};
    } else {
        size_t start_t = prev.find("<title>");
        size_t end_t = prev.find("</title>");
        if (end_t != string::npos && end_t > start_t) {
            title = prev.substr(start_t + 7, end_t - start_t - 7);
        } else {
            title = "Server Error";
        }
    }
    size_t start_b = prev.find("<body>");
    size_t end_b = prev.find("<hr>");
    if (start_b != string::npos && end_b > start_b) {
        body = prev.substr(start_b + 6, end_b - start_b - 6);
    } else {
        body = "<h1>Server Error</h1>";
    }
    html->getContext()->insert({"title", title});
    html->getContext()->insert({"content", body});
    mstch::map user;
    auto uObject = (User *)auth->getUser();
    if (uObject) {
        user.insert({"name", uObject->getName()});
        html->getContext()->insert({"user", user});
    }
    html->exec();
}
