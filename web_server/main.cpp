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
    HandlerCommon() : Handler() {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        response->setVersion(HTTP::Version::HTTP_1_1);
        if (request->getMethod() == HTTP::Method::UNDEFINED
                || request->getVersion() == HTTP::Version::HTTP_UNDEFINED) {
            response->setStatus(400);
        } else if (request->getVersion() != HTTP::Version::HTTP_1_1) {
            response->setStatus(505);
        } else {
            response->setStatus(404);
        }
    }
};

class HandlerTemplate : public Handler {
public:
    HandlerTemplate() : Handler() {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        response->getHeaders()->add("Content-Type", "text/html; charset=utf-8");
        std::string template_str;
        bool found_template = FileHandler::loadFile("../data/common.html", template_str);
        std::string data = "<html><head><title>";
        data += std::to_string(response->getStatus());
        data += " " + HTTP::getReasonPhrase(response->getStatus());
        data += "</title></head><body><h1>";
        data += std::to_string(response->getStatus()) + " ";
        data += HTTP::getReasonPhrase(response->getStatus());
        data += "</h1><hr><address>Web-framework (c++) 2018</address></body></html>";
        response->getBody()->setBody(data);
        if (found_template) {
            Middleware * middleware = getContext()->getMiddlewareByNameID("html");
            if (!middleware) return;
            auto * html = (HtmlMiddleware *) (void *) middleware;
            html->setView(template_str);
        }
    }
};


class HandlerCookie : public Handler {
public:
    HandlerCookie() : Handler() {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware_html = getContext()->getMiddlewareByNameID("html");
        if (!middleware_html) return;
        auto * html = (HtmlMiddleware *) (void *) middleware_html;
        Middleware * middleware_cookie = getContext()->getMiddlewareByNameID("cookie");
        if (!middleware_cookie) return;
        auto * cookie = (CookieMiddleware *) (void *) middleware_cookie;
        mstch::array receipts_num;
        std::string temp;
        for (int i = 1; i <= 4; i++) {
            std::string varName = "receipts_num";
            varName += std::to_string(i);
            if (cookie->getValueFromMap(varName.c_str(), temp)) {
                mstch::map current;
                current.insert({"num", temp});
                receipts_num.push_back(current);
            }
        }
        html->getContext()->insert({"history_receipt", receipts_num});
    }
};


class HandlerRenderTemplate : public Handler {
public:
    HandlerRenderTemplate() : Handler() {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        // !!
        std::vector<std::vector<std::string>> result_pages_nav_bar;
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
        // !

        // !
        std::vector<std::vector<std::string>> result_pages_menu;
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
        // !

        // !!
        std::vector<std::vector<std::string>> result_uri_receipt;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'uri_receipt'",
                result_uri_receipt,
                nullptr,
                0)
        ) return;
        if (result_uri_receipt.size() != 1 || result_uri_receipt[0].size() != 1) return;
        html->getContext()->insert({"uri_receipt", std::string{result_uri_receipt[0][0]}});
        // !!

        // !!
        if (request->getURI()->getPath().empty()) return;
        char * data[] = { (char *) request->getURI()->getPath().c_str()};
        std::vector<std::vector<std::string>> result_title;
        if (!db->execQuery(
                "SELECT title FROM pages WHERE link = ?",
                result_title,
                data,
                1)
        ) return;
        // !!

        std::string prev = response->getBody()->getBody();
        std::string title;
        std::string body;
        if (result_title.size() == 1 && result_title[0].size() == 1)  {
            title = std::string{result_title[0][0]};
        } else {
            size_t start_t = prev.find("<title>");
            size_t end_t = prev.find("</title>");
            if (end_t != std::string::npos && end_t > start_t) {
                title = prev.substr(start_t + 7, end_t - start_t - 7);
            } else {
                title = "Server Error";
            }
        }
        size_t start_b = prev.find("<body>");
        size_t end_b = prev.find("<hr>");
        if (start_b != std::string::npos && end_b > start_b) {
            body = prev.substr(start_b + 6, end_b - start_b - 6);
        } else {
            body = "<h1>Server Error</h1>";
        }
        html->getContext()->insert({"title", title});
        html->getContext()->insert({"content", body});
        html->exec();
    }
};

class HandlerIndex : public Handler {
public:
    HandlerIndex(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_index;
        if (!FileHandler::loadFile("../data/index.html", template_index)) return;
        mstch::map index_content;
        index_content.insert({"header", std::string{"Logistic company #1"}});

        // !!
        std::vector<std::vector<std::string>> result_text;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'index_text'",
                result_text,
                nullptr,
                0)
        ) return;
        if (result_text.size() != 1 || result_text[0].size() != 1) return;
        //
        index_content.insert({"text", result_text[0][0]});

        std::string content = mstch::render(template_index, index_content);
        body->second = content;
        html->exec();
    }
};


class HandlerTrack : public Handler {
public:
    HandlerTrack(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_track;
        if (!FileHandler::loadFile("../data/track.html", template_track)) return;
        mstch::map track_content;

        std::string num;
        if (!request->getURI()->getValueFromParam("cargo_number", num)) {
            std::string content = mstch::render(template_track, track_content);
            body->second = content;
            html->exec();
            return;
        }

        char * data[] = {(char *)num.c_str()};
        std::vector<std::vector<std::string>> result_receipt;
        if (!db->execQuery(
                "SELECT receipts.sender, receipts.receiver, receipts.date_dep, receipts.date_arr, "
                "cities.name, receipts.price FROM receipts INNER JOIN cities "
                "ON receipts.city_id = cities.id WHERE num = ?",
                result_receipt,
                data,
                1)
        ) return;
        if (result_receipt.size() != 1 || result_receipt[0].size() != 6) {
            std::string content = mstch::render(template_track, track_content);
            body->second = content;
            html->exec();
            return;
        }

        //
        Middleware * middleware_cookie = getContext()->getMiddlewareByNameID("cookie");
        if (!middleware_cookie) return;
        auto * cookie = (CookieMiddleware *) (void *) middleware_cookie;
        std::string temp;
        bool toShift = false;
        for (int i = 1; i <= 4; i++) {
            std::string varName = "receipts_num";
            varName += std::to_string(i);
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
            std::string varName = "receipts_num" + std::to_string(i);
            std::string varNamePrev = "receipts_num" + std::to_string(i - 1);
            std::string prevVal;
            cookie->getValueFromMap(varNamePrev.c_str(), prevVal);
            CookieEntity cur{prevVal.c_str()};
            cookie->addCookie(varName.c_str(), cur);
        }
        if (toShift) {
            CookieEntity cur{num.c_str()};
            cookie->addCookie("receipts_num1", cur);
        }
        cookie->insertInResponse();
        //


        //
        std::string date_dep = result_receipt[0][2];
        std::time_t dep = std::stoi(date_dep);
        std::tm * ptm = std::localtime(&dep);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%A, %d.%m.%Y %H:%M:%S", ptm);
        result_receipt[0][2] = buffer;
        //

        //
        std::string date_arr = result_receipt[0][3];
        std::time_t arr = std::stoi(date_arr);
        std::tm * ptm2 = std::localtime(&arr);
        char buffer2[100];
        std::strftime(buffer2, sizeof(buffer2), "%A, %d.%m.%Y %H:%M:%S", ptm2);
        result_receipt[0][3] = buffer2;
        //


        mstch::array array_table_receipt;
        const char * names_table[] ={"Номер відправлення", "Відправник", "Отримувач",
                                     "Дата відправленя", "Дата прибуття", "Поточне місцезнаходження",
                                     "Вартість, грн."};
        for (int i = 0; i < sizeof(names_table) / sizeof(const char *); i++) {
            mstch::map cur{{"name", std::string{names_table[i]}}, {"value", i == 0 ? std::string{num} : std::string{result_receipt[0][i - 1]}}};
            array_table_receipt.push_back(cur);
        }

        track_content.insert({"receipt", array_table_receipt});
        std::string content = mstch::render(template_track, track_content);
        body->second = content;
        html->exec();
    }
};

class HandlerCalculate : public Handler {
public:
    HandlerCalculate(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_calculate;
        if (!FileHandler::loadFile("../data/calculate.html", template_calculate)) return;
        mstch::map calculate_content;

        std::vector<std::vector<std::string>> result_cities;
        if (!db->execQuery(
                "SELECT id, name FROM cities",
                result_cities,
                nullptr,
                0)
        ) return;


        mstch::array array_table_cities;
        for (auto & it : result_cities) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}};
            array_table_cities.push_back(cur);
        }


        //
        std::vector<std::vector<std::string>> result_cargo_types;
        if (!db->execQuery(
                "SELECT id, name FROM cargo_types",
                result_cargo_types,
                nullptr,
                0)
        ) return;


        mstch::array array_cargo_types;
        for (auto & it : result_cargo_types) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}};
            array_cargo_types.push_back(cur);
        }

        //


        // !!
        std::vector<std::vector<std::string>> result_uri;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'uri_calculate'",
                result_uri,
                nullptr,
                0)
        ) return;
        if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

        calculate_content.insert({"uri_calculate", result_uri[0][0]});

        calculate_content.insert({"cities", array_table_cities});
        calculate_content.insert({"cargo_types", array_cargo_types});
        std::string content = mstch::render(template_calculate, calculate_content);
        body->second = content;
        html->exec();
    }
};

static std::string toString(double val, int precision = 2) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << val;
    return stream.str();
}

class HandlerCalculatePost : public Handler {
public:
    HandlerCalculatePost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
        if (!middleware_form) return;
        auto * form = (FormMiddleware *) (void *) middleware_form;

        bool error = false;

        std::string departure_str;
        std::string arrive_str;
        std::string weight_str;
        std::string height_str;
        std::string length_str;
        std::string width_str;
        std::string cargo_str;

        if (!form->getValueFromMap("departure", departure_str)) error = true;
        if (!form->getValueFromMap("arrive", arrive_str)) error = true;
        if (!form->getValueFromMap("weight", weight_str)) error = true;
        if (!form->getValueFromMap("height", height_str)) error = true;
        if (!form->getValueFromMap("length", length_str)) error = true;
        if (!form->getValueFromMap("width", width_str)) error = true;
        if (!form->getValueFromMap("cargo_type", cargo_str)) error = true;

        std::vector<std::vector<std::string>> result_prices_config;
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
        std::vector<std::vector<std::string>> result_price_city;
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
        std::vector<std::vector<std::string>> result_cargo_conf;
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
            weight = std::stod(weight_str);
            height = std::stod(height_str);
            length = std::stod(length_str);
            width = std::stod(width_str);
            weight_config = std::stod(result_prices_config[0][0]);
            volume_config = std::stod(result_prices_config[1][0]);
            city_tax_first = std::stod(result_price_city[0][0]);
            cargo_config = std::stod(result_cargo_conf[0][0]);
            if (inCity) {
                city_tax_second = city_tax_first;
            } else {
                city_tax_second = std::stod(result_price_city[1][0]);
            }
        } catch (std::invalid_argument & err) {
            error = true;
        } catch (std::out_of_range & err) {
            error = true;
        }

        if (weight <= 0 || height <= 0 || length <= 0 || width <= 0) error = true;

        std::string template_info;
        if (!FileHandler::loadFile("../data/info.html", template_info)) return;
        mstch::map info_content{{"header", std::string{"Вартість доставки"}}};

        if (error) {
            info_content.insert({"content", std::string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}});
        } else {
            double price = weight * weight_config + ((length * height * width) / 1e6) * volume_config + fabs(city_tax_first - city_tax_second) * weight / 10;
            price *= cargo_config;
            std::string body = "<p>Орієнтована вартість доставки становить <span class=\"rate\">";
            body += toString(price) + "</span> грн.</p><p><i>Введена вага: ";
            body += toString(weight) + " кг, об'єм: " + toString((length * height * width)/ 1e6, 4) + " м<sup>3</sup></i></p>";
            info_content.insert({"content", body});
        }

        auto body = html->getContext()->find("content");
        std::string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    }
};


class HandlerEstimate : public Handler {
public:
    HandlerEstimate(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_calculate;
        if (!FileHandler::loadFile("../data/estimate.html", template_calculate)) return;
        mstch::map calculate_content;

        std::vector<std::vector<std::string>> result_cities;
        if (!db->execQuery(
                "SELECT id, name FROM cities",
                result_cities,
                nullptr,
                0)
        ) return;


        mstch::array array_table_cities;
        for (auto & it : result_cities) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}};
            array_table_cities.push_back(cur);
        }


        // !!
        std::vector<std::vector<std::string>> result_uri;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'uri_estimate'",
                result_uri,
                nullptr,
                0)
                ) return;
        if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

        calculate_content.insert({"uri_estimate", result_uri[0][0]});

        calculate_content.insert({"cities", array_table_cities});

        std::string content = mstch::render(template_calculate, calculate_content);
        body->second = content;
        html->exec();
    }
};

static std::time_t toTime_t(std::string & date, const char * format = "%Y-%m-%d") {
    std::tm tm{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, format);
    std::time_t time = mktime(&tm);
    return time;
}


class HandlerEstimatePost : public Handler {
public:
    HandlerEstimatePost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
        if (!middleware_form) return;
        auto * form = (FormMiddleware *) (void *) middleware_form;

        bool error = false;

        std::string arrive_city_str;
        std::string departure_city_str;
        std::string departure_date_str;

        if (!form->getValueFromMap("arrive", arrive_city_str)) error = true;
        if (!form->getValueFromMap("departure_city", departure_city_str)) error = true;
        if (!form->getValueFromMap("departure_date", departure_date_str)) error = true;

        std::time_t time_departure = toTime_t(departure_date_str);


        bool inCity = departure_city_str == arrive_city_str;

        char * data[] = { (char *)arrive_city_str.c_str(), (char *)departure_city_str.c_str()};
        std::vector<std::vector<std::string>> result_dates_city;
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
            days_city_arrive = std::stoi(result_dates_city[0][0]);
            days_city_departure = inCity ? days_city_arrive - 3 : std::stoi(result_dates_city[1][0]);
        } catch (std::invalid_argument & err) {
            error = true;
        } catch (std::out_of_range & err) {
            error = true;
        }

        std::string template_info;
        if (!FileHandler::loadFile("../data/info.html", template_info)) return;
        mstch::map info_content{{"header", std::string{"Термін доставки вантажу"}}};

        if (error) {
            info_content.insert({"content", std::string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}});
        } else {

            std::tm * tm = std::localtime(&time_departure);
            tm->tm_mday += abs(days_city_arrive - days_city_departure);
            mktime(tm);

            char buffer[100];
            std::strftime(buffer, sizeof(buffer), "%d.%m.%Y", tm);
            std::string arrive_str = buffer;

            std::string body = "<p>Орієнтована дата доставки: <span class=\"rate\">";
            body += arrive_str + "</span></p><p><i>Доставка вантажу триватиме ";
            if (inCity) {
                body += "по місту <b>" + result_dates_city[0][1] + "</b> ";
            } else {
                body += "з <b>" + result_dates_city[0][1] + "</b> до <b>" + result_dates_city[1][1] + "</b> ";
            }
            body += "<b>" + std::to_string(abs(days_city_arrive - days_city_departure)) + "</b> днів(-і)";
            info_content.insert({"content", body});
        }
        auto body = html->getContext()->find("content");
        std::string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    }
};


class HandlerMap : public Handler {
public:
    HandlerMap(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_map;
        if (!FileHandler::loadFile("../data/map.html", template_map)) return;
        mstch::map map_content;

        std::string curTarget;
        request->getURI()->getValueFromParam("target", curTarget);

        std::vector<std::vector<std::string>> result_cities;
        if (!db->execQuery(
                "SELECT id, name FROM cities",
                result_cities,
                nullptr,
                0)
                ) return;


        mstch::array array_table_cities;
        for (auto & it : result_cities) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}, {"selected", it[0].insert(0, "city-") == curTarget}};
            array_table_cities.push_back(cur);
        }


        std::vector<std::vector<std::string>> result_countries;
        if (!db->execQuery(
                "SELECT id, name FROM districts",
                result_countries,
                nullptr,
                0)
                ) return;


        mstch::array array_table_countries;
        for (auto & it : result_countries) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}, {"selected", it[0].insert(0, "country-") == curTarget}};
            array_table_countries.push_back(cur);
        }

        std::string city_id = "0";
        std::string country_id = "0";
        size_t endKeyCityPos = curTarget.find("city-");
        size_t endKeyCountryPos = curTarget.find("country-");
        if (endKeyCityPos == 0) {
            city_id = curTarget.substr(sizeof("city-") - 1);
        } else if (endKeyCountryPos == 0) {
            country_id = curTarget.substr(sizeof("country-") - 1);
        }

        char * data[] = { (char *)city_id.c_str(), (char *)country_id.c_str() };
        std::vector<std::vector<std::string>> table;
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
            mstch::map cur{{"num", it[0]}, {"city", it[1]}, {"country", it[2]},
                           {"address", it[3]}, {"timetable", "Пн - Пт: " + it[4] + std::string{"\nСб - Нд: "} + it[5]}};
            array_departments.push_back(cur);
        }




        map_content.insert({"departments", array_departments});
        map_content.insert({"error", array_departments.empty() ? !curTarget.empty() : false});
        map_content.insert({"uri_map", request->getURI()->getPath()});
        map_content.insert({"cities", array_table_cities});
        map_content.insert({"countries", array_table_countries});

        std::string content = mstch::render(template_map, map_content);
        body->second = content;
        html->exec();
    }
};



class HandlerOrder : public Handler {
public:
    HandlerOrder(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_order;
        if (!FileHandler::loadFile("../data/order.html", template_order)) return;
        mstch::map order_content;

        std::vector<std::vector<std::string>> result_cities;
        if (!db->execQuery(
                "SELECT id, name FROM cities",
                result_cities,
                nullptr,
                0)
        ) return;


        mstch::array array_table_cities;
        for (auto & it : result_cities) {
            if (it.size() != 2) continue;
            mstch::map cur{{"id", it[0]}, {"name", it[1]}};
            array_table_cities.push_back(cur);
        }


        // !!
        std::vector<std::vector<std::string>> result_uri;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'uri_order'",
                result_uri,
                nullptr,
                0)
        ) return;
        if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

        order_content.insert({"uri_order", result_uri[0][0]});

        order_content.insert({"cities", array_table_cities});

        std::string content = mstch::render(template_order, order_content);
        body->second = content;
        html->exec();
    }
};


class HandlerOrderPost : public Handler {
public:
    HandlerOrderPost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
        if (!middleware_form) return;
        auto * form = (FormMiddleware *) (void *) middleware_form;

        bool error = false;

        std::string city_str;
        std::string name_str;
        std::string email_str;
        std::string phone_str;
        std::string address_str;
        std::string comment_str;

        if (!form->getValueFromMap("city", city_str)) error = true;
        if (!form->getValueFromMap("name", name_str)) error = true;
        if (!form->getValueFromMap("email", email_str)) error = true;
        if (!form->getValueFromMap("phone", phone_str)) error = true;
        if (!form->getValueFromMap("address", address_str)) error = true;
        if (!form->getValueFromMap("comment", comment_str)) error = true;


        time_t raw_time;
        tm * time_cur;
        char time_str[100];
        time(&raw_time);
        time_cur = localtime(&raw_time);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_cur);


        const char * data[] = {
                city_str.c_str(),
                name_str.c_str(),
                email_str.c_str(),
                phone_str.c_str(),
                address_str.c_str(),
                comment_str.size() == 0 ? "NULL" : comment_str.c_str(),
                time_str
        };


        std::vector<std::vector<std::string>> res_query;
        if (!db->execQuery(
                "INSERT INTO orders (`city_id`, `name`,`email`,`phone`,`address`,`comment`,`date_order`) "
                "VALUES (?, ?, ?, ?, ?, ?, ?);",
                res_query,
                (char **)data,
                7)
        ) return;

        if (!res_query.empty()) error = true;


        std::string template_info;
        if (!FileHandler::loadFile("../data/info.html", template_info)) return;
        mstch::map info_content{{"header", std::string{"Результат заповнення заявки"}}};

        if (error) {
            info_content.insert({"content", std::string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}});
        } else {
            info_content.insert({"content", std::string{"<p>Дякуємо, <b>" + name_str + "</b>, вашу заявку прийнято!"}});
        }
        auto body = html->getContext()->find("content");
        std::string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    }
};


class HandlerCommonInfo : public Handler {
    std::string key_in_db;
public:
    HandlerCommonInfo(const char * key_in_db, const char * ds, HTTP::Method m) : Handler(ds, m) {
        this->key_in_db = key_in_db;
    }
    const char * getKey() { return key_in_db.c_str(); }
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
        if (!middleware_form) return;
        auto * form = (FormMiddleware *) (void *) middleware_form;



        const char * data[] = {getKey()};
        std::vector<std::vector<std::string>> result_data;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = ?",
                result_data,
                (char **)data,
                1)
        ) return;
        if (result_data.size() != 1 || result_data[0].size() != 1) return;


        const char * uri[] = {request->getURI()->getPath().c_str()};
        std::vector<std::vector<std::string>> result_header;
        if (!db->execQuery(
                "SELECT title FROM pages WHERE link = ?",
                result_header,
                (char **)uri,
                1)
        ) return;
        if (result_header.size() != 1 || result_header[0].size() != 1) return;


        std::string template_info;
        if (!FileHandler::loadFile("../data/info.html", template_info)) return;
        mstch::map info_content{{"header", result_header[0][0]}};


        info_content.insert({"content", result_data[0][0]});

        auto body = html->getContext()->find("content");
        std::string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    }
};


class HandlerNews : public Handler {
public:
    HandlerNews(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_news;
        if (!FileHandler::loadFile("../data/news.html", template_news)) return;
        mstch::map news_content;


        news_content.insert({"uri_news", request->getURI()->getPath()});


        std::string currentNewspaper;

        if (request->getURI()->getValueFromParam("current", currentNewspaper)) {


            const char * dat[] = {currentNewspaper.c_str()};
            std::vector<std::vector<std::string>> article;
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

            std::time_t date = std::stoi(article[0][3]);
            std::tm * ptm = std::localtime(&date);
            char f_date[100];
            char s_date[100];
            std::strftime(f_date, sizeof(f_date), "%Y-%m-%d %H:%M", ptm);
            std::strftime(s_date, sizeof(s_date), "%d.%m.%Y", ptm);
            article_context.insert({"full_date", std::string{f_date}});
            article_context.insert({"date", std::string{s_date}});

            news_content.insert({"newspaper", article_context});

        } else {

            std::vector<std::vector<std::string>> result_news;
            if (!db->execQuery(
                    "SELECT id, header, text, date FROM news",
                    result_news,
                    nullptr,
                    0)
            ) return;

            mstch::array array_news;
            for (auto & it : result_news) {
                if (it.size() != 4) continue;

                std::time_t date = std::stoi(it[3]);
                std::tm * ptm = std::localtime(&date);
                char f_date[100];
                char s_date[100];
                std::strftime(f_date, sizeof(f_date), "%Y-%m-%d %H:%M", ptm);
                std::strftime(s_date, sizeof(s_date), "%b %d", ptm);

                mstch::map cur{{"id", it[0]}, {"title", it[1]}, {"content", it[2]},
                               {"full_date", std::string{f_date}}, {"date", std::string{s_date}}};
                array_news.push_back(cur);
            }
            news_content.insert({"news", array_news});
        }


        body->second = mstch::render(template_news, news_content);
        html->exec();
    }
};


class HandlerFeedback : public Handler {
public:
    HandlerFeedback(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        auto body = html->getContext()->find("content");

        std::string template_order;
        if (!FileHandler::loadFile("../data/feedback.html", template_order)) return;
        mstch::map order_content;




        // !!
        std::vector<std::vector<std::string>> result_uri;
        if (!db->execQuery(
                "SELECT data FROM site WHERE value = 'uri_feedback_post'",
                result_uri,
                nullptr,
                0)
        ) return;
        if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

        order_content.insert({"uri_feedback", result_uri[0][0]});


        std::string content = mstch::render(template_order, order_content);
        body->second = content;
        html->exec();
    }
};


class HandlerFeedbackPost : public Handler {
public:
    HandlerFeedbackPost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() {
        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
        if (!middleware_form) return;
        auto * form = (FormMiddleware *) (void *) middleware_form;

        bool error = false;

        std::string name_str;
        std::string email_str;
        std::string comment_str;

        if (!form->getValueFromMap("name", name_str)) error = true;
        if (!form->getValueFromMap("email", email_str)) error = true;
        if (!form->getValueFromMap("comment", comment_str)) error = true;





        const char * data[] = {
                name_str.c_str(),
                email_str.c_str(),
                comment_str.c_str()
        };


        std::vector<std::vector<std::string>> res_query;
        if (!db->execQuery(
                "INSERT INTO reviews ( `name`,`email`,`text`) "
                "VALUES (?, ?, ?);",
                res_query,
                (char **)data,
                3)
        ) return;

        if (!res_query.empty()) error = true;


        std::string template_info;
        if (!FileHandler::loadFile("../data/info.html", template_info)) return;
        mstch::map info_content{{"header", std::string{"Feedback"}}};

        if (error) {
            info_content.insert({"content", std::string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}});
        } else {
            info_content.insert({"content", std::string{"<p>Дякуємо, <b>" + name_str + "</b>, вашу відгук буде врахован!"}});
        }
        auto body = html->getContext()->find("content");
        std::string content = mstch::render(template_info, info_content);
        body->second = content;
        html->exec();
    }
};


class HandlerApi : public Handler {
public:
    HandlerApi(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {

        Request * request = getContext()->getRequest();
        Response * response = getContext()->getResponse();
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        DBManager * db = getContext()->getDB();
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        if (html->getView().empty()) return;

        Middleware * m_json = this->getContext()->getMiddlewareByNameID("json");

        if (!m_json) return;

        auto * json = (JsonMiddleware *) (void *) m_json;

        if (json->getJsonRequest()->empty()) {

            std::string template_info;
            if (!FileHandler::loadFile("../data/info.html", template_info)) return;
            mstch::map info_content{{"header", std::string{"API"}}};


            info_content.insert({"content", std::string{"<p>API версії 1.1.0 дозволяє отримувати дані про вантаж "
                                                        "за його номером у форматі <code>json</code>.</p>"
                                                        "<p>Для цього на сторінку <code>/api</code> відправте <code>json</code> об'єкт із полем "
                                                        " <code>cargo_number</code> значенням якого є номер шуканого відправлення. У "
                                                        "відповідь отримаєте <code>json</code> об'єкт із полем <code>status</code>, у разі <code>false</code> "
                                                        "означатиме, що даного номера немає в базі. В противному випадку"
                                                        " дані будуть представлені, як в прикладі нижче:<br>"
                                                        "<code>{\n"
                                                        "    \"current_city\": \"New York\",\n"
                                                        "    \"date_arrive\": \"Sunday, 03.06.2018 21:54:12\",\n"
                                                        "    \"date_departure\": \"Friday, 01.06.2018 15:23:34\",\n"
                                                        "    \"num\": 568903,\n"
                                                        "    \"price\": 234.46,\n"
                                                        "    \"receiver\": \"John Doe\",\n"
                                                        "    \"sender\": \"Uncle Tom\",\n"
                                                        "    \"status\": true\n"
                                                        "}</code>"}});

            auto body = html->getContext()->find("content");
            std::string content = mstch::render(template_info, info_content);
            body->second = content;
            html->exec();


        } else {
            (*json->getJsonResponse())["status"] = false;

            int num;

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




            char * data[] = {(char *)std::to_string(number).c_str()};
            std::vector<std::vector<std::string>> result_receipt;
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


            std::string date_dep = result_receipt[0][2];
            std::time_t dep = std::stoi(date_dep);
            std::tm * ptm = std::localtime(&dep);
            char buffer[100];
            std::strftime(buffer, sizeof(buffer), "%A, %d.%m.%Y %H:%M:%S", ptm);
            result_receipt[0][2] = buffer;
            //

            //
            std::string date_arr = result_receipt[0][3];
            std::time_t arr = std::stoi(date_arr);
            std::tm * ptm2 = std::localtime(&arr);
            char buffer2[100];
            std::strftime(buffer2, sizeof(buffer2), "%A, %d.%m.%Y %H:%M:%S", ptm2);
            result_receipt[0][3] = buffer2;
            //


            (*json->getJsonResponse())["status"] = true;
            (*json->getJsonResponse())["sender"] = result_receipt[0][0];
            (*json->getJsonResponse())["receiver"] = result_receipt[0][1];
            (*json->getJsonResponse())["date_departure"] = result_receipt[0][2];
            (*json->getJsonResponse())["date_arrive"] = result_receipt[0][3];
            (*json->getJsonResponse())["current_city"] = result_receipt[0][4];
            (*json->getJsonResponse())["price"] = std::stod(result_receipt[0][5]);
            (*json->getJsonResponse())["num"] = number;

            json->fillResponse();
        }

    }
};

class HandlerExit : public Handler {
public:
    HandlerExit(const char * ds, HTTP::Method m) :Handler(ds, m) {}
    void exec() {
        this->getContext()->emitCloseEvent();
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


    HandlerCommon * dada = new HandlerCommon();
    HandlerTemplate * sdf = new HandlerTemplate();
    HandlerCookie * xcv =  new HandlerCookie();
    HandlerRenderTemplate * sdfsd = new HandlerRenderTemplate();

    HandlerIndex * index = new HandlerIndex("/", HTTP::Method::GET);
    HandlerTrack * track = new HandlerTrack("/track", HTTP::Method::GET);
    HandlerCalculate * calc = new HandlerCalculate("/calculate", HTTP::Method::GET);
    HandlerCalculatePost * post = new HandlerCalculatePost("/calculate_post", HTTP::Method::POST);
    HandlerEstimate * est = new HandlerEstimate("/estimate", HTTP::Method::GET);
    HandlerEstimatePost * est_post = new HandlerEstimatePost("/estimate_post", HTTP::Method::POST);
    HandlerMap * map = new HandlerMap("/map", HTTP::Method::GET);
    HandlerOrder * order = new HandlerOrder("/order", HTTP::Method::GET);
    HandlerOrderPost * post_order = new HandlerOrderPost("/order_post", HTTP::Method::POST);
    HandlerCommonInfo * about = new HandlerCommonInfo("about_content", "/about", HTTP::Method::GET);
    HandlerCommonInfo * privacy = new HandlerCommonInfo("privacy_content", "/privacy_policy", HTTP::Method::GET);
    HandlerNews * news = new HandlerNews("/news", HTTP::Method::GET);
    HandlerFeedback * feed = new HandlerFeedback("/feedback", HTTP::Method::GET);
    HandlerFeedbackPost * feed_post = new HandlerFeedbackPost("/feedback_post",  HTTP::Method::POST);
    HandlerCommonInfo * time = new HandlerCommonInfo("timetable_content", "/timetable", HTTP::Method::GET);


    HandlerExit * xxxxxx = new HandlerExit("/exit", HTTP::Method::GET);
    website.addHandler(xxxxxx);
    //

    HandlerApi * nbv = new HandlerApi("/api", HTTP::Method::ANY);
    //HandlerCookie * cookies = new HandlerCookie("/cookie", HTTP::Method::GET);
    //HandlerTrack * track = new HandlerTrack("/db", HTTP::Method::GET);

    FileHandler * css = new FileHandler("/common.css", "../data/common.css", "text/css", false);
    FileHandler * img = new FileHandler("/logo.jpg", "../data/logo.jpg", "image/jpeg", true);
    FileHandler * banner = new FileHandler("/banner.jpg", "../data/banner.jpg", "image/jpeg", true);
    FileHandler * img_news = new FileHandler("/news.png", "../data/news.png", "image/png", true);

    website.addHandler(dada);
    website.addHandler(sdf);
    website.addHandler(xcv);
    website.addHandler(sdfsd);

    website.addHandler(index);
    website.addHandler(track);
    website.addHandler(calc);
    website.addHandler(post);
    website.addHandler(est);
    website.addHandler(est_post);
    website.addHandler(map);
    website.addHandler(order);
    website.addHandler(post_order);
    website.addHandler(about);
    website.addHandler(privacy);
    website.addHandler(news);
    website.addHandler(feed);
    website.addHandler(feed_post);
    website.addHandler(time);
    ///
    website.addHandler(nbv);
    website.addHandler(track);
    website.addHandler(css);
    website.addHandler(img);
    website.addHandler(banner);
    website.addHandler(img_news);


    website.addPermanentlyRedirect("/index", "/");
    website.addPermanentlyRedirect("/index.html", "/");
    website.addPermanentlyRedirect("/index.php", "/");
    website.addPermanentlyRedirect("/old", "/main");

    JsonMiddleware * json = new JsonMiddleware("json");
    CookieMiddleware * cookie = new CookieMiddleware("cookie");
    FormMiddleware * form = new FormMiddleware("form");
    HtmlMiddleware * html = new HtmlMiddleware("html");

    website.addMiddleware(json);
    website.addMiddleware(cookie);
    website.addMiddleware(form);
    website.addMiddleware(html);

    website.run();
    return 0;
}
