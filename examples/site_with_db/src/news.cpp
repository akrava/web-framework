#include <news.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerNews::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    auto body = html->getContext()->find("content");

    string template_news;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/news.mustache").c_str(), template_news)) return;
    mstch::map news_content;

    news_content.insert({"uri_news", request->getURI()->getPath()});
    string currentNewspaper;

    if (request->getURI()->getValueFromParam("current", currentNewspaper)) {
        const char * dat[] = {currentNewspaper.c_str()};
        vector<vector<string>> article;
        if (!db->execQuery(
                "SELECT header, img, text, date FROM news WHERE id = ?",
                article,
                (char **)dat,
                1)
        ) return;

        if (article.size() != 1 || article[0].size() != 4) return;

        mstch::map article_context{{"title", article[0][0]}, {"content", article[0][2]}};

        if (!article[0][1].empty()) {
            article_context.insert({"img", article[0][1]});
        }

        time_t date = stoi(article[0][3]);
        string f_date = ParserHTTP::getTime(&date, "%Y-%m-%d %H:%M");
        string s_date = ParserHTTP::getTime(&date, "%d.%m.%Y");
        article_context.insert({"full_date", f_date});
        article_context.insert({"date", s_date});
        news_content.insert({"newspaper", article_context});
    } else {
        vector<vector<string>> result_news;
        if (!db->execQuery(
                "SELECT id, header, text, date FROM news",
                result_news,
                nullptr,
                0)
        ) return;

        mstch::array array_news;
        for (auto & it : result_news) {
            if (it.size() != 4) continue;
            time_t date = stoi(it[3]);
            string f_date = ParserHTTP::getTime(&date, "%Y-%m-%d %H:%M");
            string s_date = ParserHTTP::getTime(&date, "%b %d");

            mstch::map cur{{"id", it[0]}, {"title", it[1]}, {"content", it[2]},
                            {"full_date", f_date}, {"date", s_date}};
            array_news.push_back(cur);
        }
        news_content.insert({"news", array_news});
    }
    body->second = mstch::render(template_news, news_content);
    html->exec();
}