#include <estimate_post.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>
#include <ctime>

using namespace std;

void HandlerEstimatePost::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;
    auto * form = (FormMiddleware *) (void *) middleware_form;

    bool error = false;
    string arrive_city_str;
    string departure_city_str;
    string departure_date_str;

    if (!form->getValueFromMap("arrive", arrive_city_str)) error = true;
    if (!form->getValueFromMap("departure_city", departure_city_str)) error = true;
    if (!form->getValueFromMap("departure_date", departure_date_str)) error = true;

    time_t time_departure = toTime_t(departure_date_str);

    bool inCity = departure_city_str == arrive_city_str;

    char * data[] = { (char *)arrive_city_str.c_str(), (char *)departure_city_str.c_str()};
    vector<vector<string>> result_dates_city;
    if (!db->execQuery(
            "SELECT warehouse_days, name FROM cities WHERE id = ? OR id = ?",
            result_dates_city,
            data,
            2)
    ) return;

    if ((!inCity && (result_dates_city.size() != 2 || result_dates_city[0].size() != 2
        || result_dates_city[1].size() != 2))
        || (inCity && (result_dates_city.size() != 1 || result_dates_city[0].size() != 2))) {
        return;
    }

    int days_city_arrive = 0;
    int days_city_departure = 0;

    try {
        days_city_arrive = stoi(result_dates_city[0][0]);
        days_city_departure = inCity ? days_city_arrive - 3 : stoi(result_dates_city[1][0]);
    } catch (invalid_argument & err) {
        error = true;
    } catch (out_of_range & err) {
        error = true;
    }

    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"info.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Термін доставки вантажу"}}};

    if (error) {
        info_content.insert({
                    "content",
                    string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}
        });
    } else {
        tm * tm = localtime(&time_departure);
        tm->tm_mday += abs(days_city_arrive - days_city_departure);
        mktime(tm);
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y", tm);
        string arrive_str = buffer;

        string body = "<p>Орієнтована дата доставки: <span class=\"rate\">";
        body += arrive_str + "</span></p><p><i>Доставка вантажу триватиме ";
        if (inCity) {
            body += "по місту <b>" + result_dates_city[0][1] + "</b> ";
        } else {
            body += "з <b>" + result_dates_city[0][1] + "</b> до <b>" + result_dates_city[1][1]
                                                                                            + "</b> ";
        }
        body += "<b>" + to_string(abs(days_city_arrive - days_city_departure)) + "</b> днів(-і)";
        info_content.insert({"content", body});
    }
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}