#include <common_info.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerCommonInfo::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    const char * data[] = {getKey()};
    vector<vector<string>> result_data;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = ?",
            result_data,
            (char **)data,
            1)
    ) return;
    if (result_data.size() != 1 || result_data[0].size() != 1) return;

    const char * uri[] = {request->getURI()->getPath().c_str()};
    vector<vector<string>> result_header;
    if (!db->execQuery(
            "SELECT title FROM pages WHERE link = ?",
            result_header,
            (char **)uri,
            1)
    ) return;
    if (result_header.size() != 1 || result_header[0].size() != 1) return;

    string template_info;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/info.mustache").c_str(), template_info)) return;
    mstch::map info_content{{"header", result_header[0][0]}};

    info_content.insert({"content", result_data[0][0]});

    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
