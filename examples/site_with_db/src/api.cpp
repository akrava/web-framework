#include <api.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/json_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerApi::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    Middleware * m_json = this->getContext()->getMiddlewareByNameID("json");
    if (!m_json) return;
    auto * json = (JsonMiddleware *) (void *) m_json;
    if (json->getJsonRequest()->empty()) {
        string template_info;
        if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/info.mustache").c_str(), template_info)) {
            return;
        }
        mstch::map info_content{{"header", string{"API"}}};


        vector<vector<string>> result_text;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'api_text'",
                result_text,
                nullptr,
                0)
        ) return;
        if (result_text.size() != 1 || result_text[0].size() != 1) return;


        info_content.insert({"content", result_text[0][0]});

        auto body = html->getContext()->find("content");
        string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    } else {

        (*json->getJsonResponse())["status"] = false;

        if (!json->getJsonRequest()->is_object()) {
            json->fillResponse();
            return;
        }
        auto it = json->getJsonRequest()->find("cargo_number");
        if (it == json->getJsonRequest()->end()) {
            json->fillResponse();
            return;
        }

        nlohmann::json obj = (*json->getJsonRequest())["cargo_number"];
        if (!obj.is_number()) {
            json->fillResponse();
            return;
        }
        int number = (*json->getJsonRequest())["cargo_number"];

        char * data[] = {(char *)to_string(number).c_str()};
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
            json->fillResponse();
            return;
        }
        json->getJsonResponse()->clear();

        time_t dep = stoi(result_receipt[0][2]);
        result_receipt[0][2] = ParserHTTP::getTime(&dep, "%A, %d.%m.%Y %H:%M:%S");

        time_t arr = stoi(result_receipt[0][3]);
        result_receipt[0][3] = ParserHTTP::getTime(&arr, "%A, %d.%m.%Y %H:%M:%S");
        (*json->getJsonResponse())["status"] = true;
        (*json->getJsonResponse())["sender"] = result_receipt[0][0];
        (*json->getJsonResponse())["receiver"] = result_receipt[0][1];
        (*json->getJsonResponse())["date_departure"] = result_receipt[0][2];
        (*json->getJsonResponse())["date_arrive"] = result_receipt[0][3];
        (*json->getJsonResponse())["current_city"] = result_receipt[0][4];
        (*json->getJsonResponse())["price"] = stod(result_receipt[0][5]);
        (*json->getJsonResponse())["num"] = number;
        json->fillResponse();
        Handler::exec();
    }
}