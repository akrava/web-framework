#include <cookie.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/cookie_middleware.h>
#include <config.h>

using namespace std;

void HandlerCookie::exec() {
    Middleware * middleware_html = getContext()->getMiddlewareByNameID("html");
    if (!middleware_html) return;
    auto * html = (HtmlMiddleware *) (void *) middleware_html;
    Middleware * middleware_cookie = getContext()->getMiddlewareByNameID("cookie");
    if (!middleware_cookie) return;
    auto * cookie = (CookieMiddleware *) (void *) middleware_cookie;
    mstch::array receipts_num;
    string temp;
    for (int i = 1; i <= 4; i++) {
        string varName = "receipts_num";
        varName += to_string(i);
        if (cookie->getValueFromMap(varName.c_str(), temp)) {
            mstch::map current;
            current.insert({"num", temp});
            receipts_num.push_back(current);
        }
    }
    html->getContext()->insert({"history_receipt", receipts_num});
    Handler::exec();
}
