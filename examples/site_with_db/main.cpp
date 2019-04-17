#include <akrava/web-server/app.h>
#include <runtime_exception.h>
#include <iostream>
#include <common.h>
#include <template.h>
#include <cookie.h>
#include <render_template.h>
#include <index.h>
#include <calculate.h>
#include <calculate_post.h>
#include <estimate.h>
#include <estimate_post.h>
#include <map.h>
#include <order.h>
#include <order_post.h>
#include <common_info.h>
#include <news.h>
#include <feedback.h>
#include <feedback_post.h>
#include <api.h>
#include <track.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/json_middleware.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/html_middleware.h>
#include <database_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <config.h>
#include "fs_middleware.h"

using namespace std;

int main (int argc, char ** argv) {

    InitParams cur;
    try {
        cur = InitParams(argc, argv);
    } catch (RuntimeException & err) {
        cerr << err.what() << endl;
        return EXIT_FAILURE;
    }
    App website = App(cur);
    website.init();



    website.addMiddleware(new FsMiddleware("FS", "./../src", "hmmm"));

    website.addHandler(new HandlerCommon());
    website.addHandler(new HandlerTemplate());
    website.addHandler(new HandlerCookie());
    website.addHandler(new HandlerRenderTemplate());

    website.addHandler(new HandlerIndex("/", HTTP::Method::GET));
    website.addHandler(new HandlerCalculate("/calculate", HTTP::Method::GET));
    website.addHandler(new HandlerCalculatePost("/calculate", HTTP::Method::POST));
    website.addHandler(new HandlerEstimate("/estimate", HTTP::Method::GET));
    website.addHandler(new HandlerEstimatePost("/estimate", HTTP::Method::POST));
    website.addHandler(new HandlerMap("/map", HTTP::Method::GET));
    website.addHandler(new HandlerOrder("/order", HTTP::Method::GET));
    website.addHandler(new HandlerOrderPost("/order", HTTP::Method::POST));
    website.addHandler(new HandlerCommonInfo("about_content", "/about", HTTP::Method::GET));
    website.addHandler(new HandlerCommonInfo("privacy_content", "/privacy_policy", HTTP::Method::GET));
    website.addHandler(new HandlerNews("/news", HTTP::Method::GET));
    website.addHandler(new HandlerFeedback("/feedback", HTTP::Method::GET));
    website.addHandler(new HandlerFeedbackPost("/feedback",  HTTP::Method::POST));
    website.addHandler(new HandlerCommonInfo("timetable_content", "/timetable", HTTP::Method::GET));

    website.addHandler(new HandlerApi("/api", HTTP::Method::ANY));
    website.addHandler(new HandlerTrack("/track", HTTP::Method::GET));
    website.addHandler(new FileHandler("/common.css", (currentDir() + __PATH_TO_DATA"/common.css").c_str(), "text/css", false));
    website.addHandler(new FileHandler("/logo.jpg", (currentDir() + __PATH_TO_DATA"/logo.jpg").c_str(), "image/jpeg", true));
    website.addHandler(new FileHandler("/banner.jpg", (currentDir() + __PATH_TO_DATA"/banner.jpg").c_str(), "image/jpeg", true));
    website.addHandler(new FileHandler("/news.png", (currentDir() + __PATH_TO_DATA"/news.png").c_str(), "image/png", true));

    website.addPermanentlyRedirect("/index", "/");

    website.addMiddleware(new JsonMiddleware("json"));
    website.addMiddleware(new CookieMiddleware("cookie"));
    website.addMiddleware(new FormMiddleware("form"));
    website.addMiddleware(new HtmlMiddleware("html"));
    website.addMiddleware(new DatabaseMiddleware("db", (currentDir() + "/../db/db_file").c_str()));

    return website.run();
}
