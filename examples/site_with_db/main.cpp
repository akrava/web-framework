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
#include <login.h>
#include <user.h>
#include <auth.h>
#include <logout.h>
#include <receipts.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/json_middleware.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/auth_middleware.h>
#include <database_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/authorized_handler.h>
#include <akrava/web-server/cookie_auth.h>
#include <config.h>
#include "akrava/web-server/fs_middleware.h"
#include "akrava/web-server/parser_http.h"

using namespace std;

static bool checkAccess(Entity * entity);

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



    website.addMiddleware(new FsMiddleware("FS", "./../assets"));

    website.addHandler(new HandlerCommon());
    website.addHandler(new HandlerTemplate());
    website.addHandler(new HandlerCookie());
    website.addHandler(new HandlerRenderTemplate());

    website.addHandler(new HandlerLogin("/login", HTTP::Method::GET));
    website.addHandler(new HandlerLogout("/logout", HTTP::Method::GET));
    auto receipts = new HandlerReceipts("/receipts", HTTP::Method::GET);
    auto authorizedReceipts = new AuthorizedHandler(receipts, "auth", checkAccess);
    website.addHandler(authorizedReceipts);
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
    website.addHandler(new AuthHandler("/auth", HTTP::Method::POST));
    website.addHandler(new HandlerApi("/api", HTTP::Method::ANY));
    website.addHandler(new HandlerTrack("/track", HTTP::Method::GET));
    website.addPermanentlyRedirect("/index", "/");

    website.addMiddleware(new JsonMiddleware("json"));
    auto cookie = new CookieMiddleware("cookie");
    website.addMiddleware(cookie);
    website.addMiddleware(new FormMiddleware("form"));
    website.addMiddleware(new HtmlMiddleware("html"));
    auto middleware = new AuthMiddleware("auth", "cookie", "json");
    middleware->setOnSerialize(User::serialize);
    middleware->setOnDeserialize(User::deserialize);
    middleware->setOnLogin(User::checkLogin);
    middleware->setStrategy(new CookieAuth(__SECRET, cookie));
    website.addMiddleware(middleware);
    website.addMiddleware(DatabaseMiddleware::getInstance("db", (currentDir() + "/../db/db_file").c_str()));

    return website.run();
}

static bool checkAccess(Entity * entity) {
    return entity->toString() == "9caef165-76c5-4a20-8db4-8fbebca47124";
}