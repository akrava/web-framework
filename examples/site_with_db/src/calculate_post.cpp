#include <cmath>
#include <calculate_post.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/form_middleware.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerCalculatePost::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;
    auto * form = (FormMiddleware *) (void *) middleware_form;

    bool error = false;
    string departure_str;
    string arrive_str;
    string weight_str;
    string height_str;
    string length_str;
    string width_str;
    string cargo_str;

    if (!form->getValueFromMap("departure", departure_str)) error = true;
    if (!form->getValueFromMap("arrive", arrive_str)) error = true;
    if (!form->getValueFromMap("weight", weight_str)) error = true;
    if (!form->getValueFromMap("height", height_str)) error = true;
    if (!form->getValueFromMap("length", length_str)) error = true;
    if (!form->getValueFromMap("width", width_str)) error = true;
    if (!form->getValueFromMap("cargo_type", cargo_str)) error = true;

    vector<vector<string>> result_prices_config;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = 'weight_price' OR  value = 'volume_price'",
            result_prices_config,
            nullptr,
            0)
    ) return;

    if (result_prices_config.size() != 2 || result_prices_config[0].size() != 1
        || result_prices_config[1].size() != 1) {
        return;
    }

    bool inCity = departure_str == arrive_str;

    char * data[] = { (char *)departure_str.c_str(), (char *)arrive_str.c_str()};
    vector<vector<string>> result_price_city;
    if (!db->execQuery(
            "SELECT warehouse_price FROM cities WHERE id = ? OR id = ?",
            result_price_city,
            data,
            2)
    ) return;

    if ((!inCity && (result_price_city.size() != 2 || result_price_city[0].size() != 1
        || result_price_city[1].size() != 1))
        || (inCity && (result_price_city.size() != 1 || result_price_city[0].size() != 1))) {
        return;
    }

    char * temp[] = { (char *)cargo_str.c_str()};
    vector<vector<string>> result_cargo_conf;
    if (!db->execQuery(
            "SELECT price_koeff FROM cargo_types WHERE id = ?",
            result_cargo_conf,
            temp,
            1)
    ) return;

    if (result_cargo_conf.size() != 1 || result_cargo_conf[0].size() != 1) return;

    double city_tax_first = 0;
    double city_tax_second = 0;

    double weight = 0;
    double height = 0;
    double length = 0;
    double width = 0;

    double cargo_config = 0;
    double weight_config = 0;
    double volume_config = 0;
    try {
        weight = stod(weight_str);
        height = stod(height_str);
        length = stod(length_str);
        width = stod(width_str);
        weight_config = stod(result_prices_config[0][0]);
        volume_config = stod(result_prices_config[1][0]);
        city_tax_first = stod(result_price_city[0][0]);
        cargo_config = stod(result_cargo_conf[0][0]);
        if (inCity) {
            city_tax_second = city_tax_first;
        } else {
            city_tax_second = stod(result_price_city[1][0]);
        }
    } catch (invalid_argument & err) {
        error = true;
    } catch (out_of_range & err) {
        error = true;
    }

    if (weight <= 0 || height <= 0 || length <= 0 || width <= 0) error = true;

    string template_info;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/info.mustache").c_str(), template_info)) return;
    mstch::map info_content{{"header", string{"Вартість доставки"}}};

    if (error) {
        info_content.insert({
                "content",
                string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}
        });
    } else {
        double price = weight * weight_config + ((length * height * width) / 1e6) *
                volume_config + fabs(city_tax_first - city_tax_second) * weight / 10;
        price *= cargo_config;
        string body = "<p>Орієнтована вартість доставки становить <span class=\"rate\">";
        body += toString(price) + "</span> грн.</p><p><i>Введена вага: ";
        body += toString(weight) + " кг, об'єм: " + toString((length * height * width)/ 1e6, 4)
                + " м<sup>3</sup></i></p>";
        info_content.insert({"content", body});
    }

    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
    Handler::exec();
}
