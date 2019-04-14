#include <common.h>

void HandlerCommon::exec() {
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