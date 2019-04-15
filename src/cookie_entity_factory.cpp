#include <cookie_entity_factory.h>
#include <cookie_entity_default.h>
#include <cookie_entity_http.h>
#include <cookie_entity_week.h>

using namespace std;

CookieEntityFactory::CookieEntityFactory() {
    storage[Default]     = make_unique<CookieEntityDefault>("default");
    storage[HttpOnly]    = make_unique<CookieEntityHttp>("http only");
    storage[WeekExpires] = make_unique<CookieEntityWeek>("week expires");
}

unique_ptr<Entity> CookieEntityFactory::createCookie(EntityType entityType, string & value) {
    auto prototype = storage[entityType]->clone();
    prototype->setValue(value);
    return prototype;
}
