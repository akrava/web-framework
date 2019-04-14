#include <track.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerTrack::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    auto body = html->getContext()->find("content");

    string template_track;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/track.mustache").c_str(), template_track)) return;
    mstch::map track_content;

    string num;
    if (!request->getURI()->getValueFromParam("cargo_number", num)) {
        string content = mstch::render(template_track, track_content);
        body->second = content;
        html->exec();
        return;
    }

    char * data[] = {(char *)num.c_str()};
    vector<vector<string>> result_receipt;
    if (!db->execQuery(
            "SELECT receipts.sender, receipts.receiver, receipts.date_dep, receipts.date_arr, "
            "cities.name, receipts.price FROM receipts INNER JOIN cities "
            "ON receipts.city_id = cities.id WHERE num = ?",
            result_receipt,
            data,
            1)
    ) return;
    if (result_receipt.size() != 1 || result_receipt[0].size() != 6) {
        string content = mstch::render(template_track, track_content);
        body->second = content;
        html->exec();
        return;
    }
    
    Middleware * middleware_cookie = getContext()->getMiddlewareByNameID("cookie");
    if (!middleware_cookie) return;
    auto * cookie = (CookieMiddleware *) (void *) middleware_cookie;
    string temp;
    bool toShift = false;
    for (int i = 1; i <= 4; i++) {
        string varName = "receipts_num";
        varName += to_string(i);
        if (cookie->getValueFromMap(varName.c_str(), temp) && temp != num) {
            toShift = true;
        } else {
            CookieEntity cur{num.c_str()};
            cookie->addCookie(varName.c_str(), cur);
            toShift = false;
            break;
        }
    }
    for (int i = 4; i > 1 && toShift; i--) {
        string varName = "receipts_num" + to_string(i);
        string varNamePrev = "receipts_num" + to_string(i - 1);
        string prevVal;
        cookie->getValueFromMap(varNamePrev.c_str(), prevVal);
        CookieEntity cur{prevVal.c_str()};
        cookie->addCookie(varName.c_str(), cur);
    }
    if (toShift) {
        CookieEntity cur{num.c_str()};
        cookie->addCookie("receipts_num1", cur);
    }
    cookie->insertInResponse();

    time_t dep = stoi(result_receipt[0][2]);
    result_receipt[0][2] = ParserHTTP::getTime(&dep, "%A, %d.%m.%Y %H:%M:%S");

    time_t arr = stoi(result_receipt[0][3]);
    result_receipt[0][3] = ParserHTTP::getTime(&arr, "%A, %d.%m.%Y %H:%M:%S");

    mstch::array array_table_receipt;
    const char * names_table[] ={"Номер відправлення", "Відправник", "Отримувач",
                                    "Дата відправленя", "Дата прибуття", "Поточне місцезнаходження",
                                    "Вартість, грн."};
    for (size_t i = 0; i < sizeof(names_table) / sizeof(const char *); i++) {
        mstch::map cur{
            {"name", string{names_table[i]}},
            {"value", i == 0 ? string{num} : string{result_receipt[0][i - 1]}}
        };
        array_table_receipt.push_back(cur);
    }

    track_content.insert({"receipt", array_table_receipt});
    string content = mstch::render(template_track, track_content);
    body->second = content;
    html->exec();
    Handler::exec();
}