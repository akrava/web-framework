#include <map.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerMap::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    auto body = html->getContext()->find("content");

    string template_map;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/map.mustache").c_str(), template_map)) return;
    mstch::map map_content;

    string curTarget;
    request->getURI()->getValueFromParam("target", curTarget);

    vector<vector<string>> result_cities;
    if (!db->execQuery(
            "SELECT id, name FROM cities",
            result_cities,
            nullptr,
            0)
    ) return;

    mstch::array array_table_cities;
    for (auto & it : result_cities) {
        if (it.size() != 2) continue;
        mstch::map cur{
            {"id", it[0]},
            {"name", it[1]},
            {"selected", it[0].insert(0, "city-") == curTarget}
        };
        array_table_cities.push_back(cur);
    }

    vector<vector<string>> result_countries;
    if (!db->execQuery(
            "SELECT id, name FROM districts",
            result_countries,
            nullptr,
            0)
    ) return;

    mstch::array array_table_countries;
    for (auto & it : result_countries) {
        if (it.size() != 2) continue;
        mstch::map cur{
            {"id", it[0]},
            {"name", it[1]},
            {"selected", it[0].insert(0, "country-") == curTarget}
        };
        array_table_countries.push_back(cur);
    }

    string city_id = "0";
    string country_id = "0";
    size_t endKeyCityPos = curTarget.find("city-");
    size_t endKeyCountryPos = curTarget.find("country-");
    if (endKeyCityPos == 0) {
        city_id = curTarget.substr(sizeof("city-") - 1);
    } else if (endKeyCountryPos == 0) {
        country_id = curTarget.substr(sizeof("country-") - 1);
    }

    char * data[] = { (char *)city_id.c_str(), (char *)country_id.c_str() };
    vector<vector<string>> table;
    if (!db->execQuery(
            "SELECT departments.num, cities.name, districts.name, departments.address, "
            "departments.timetable_weekdays, departments.timetable_weekend FROM departments "
            "INNER JOIN cities ON departments.city_id = cities.id INNER JOIN districts ON "
            "cities.disrtict_id = districts.id WHERE cities.id = ? OR districts.id = ?",
            table,
            data,
            2)
    ) return;

    mstch::array array_departments;
    for (auto & it : table) {
        if (it.size() != 6) continue;
        mstch::map cur{
            {"num", it[0]},
            {"city", it[1]},
            {"country", it[2]},
            {"address", it[3]},
            {"timetable", "Пн - Пт: " + it[4] + string{"\nСб - Нд: "} + it[5]}
        };
        array_departments.push_back(cur);
    }

    map_content.insert({"departments", array_departments});
    map_content.insert({"error", array_departments.empty() ? !curTarget.empty() : false});
    map_content.insert({"uri_map", request->getURI()->getPath()});
    map_content.insert({"cities", array_table_cities});
    map_content.insert({"countries", array_table_countries});
    string content = mstch::render(template_map, map_content);
    body->second = content;
    html->exec();
    Handler::exec();
}
