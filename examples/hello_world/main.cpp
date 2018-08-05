#include <iostream>
#include <akrava/web-server/app.h>

using namespace std;

class HelloWorldHandler : public Handler {
public:
    void exec() {
        auto * res = new Response("Hello world");
        this->getContext()->setResponse(res);
    }
};

int main () {
    const char * IP = "127.0.0.1";
    const int PORT = 8080;
    App hello_world(IP, PORT);
    if (!hello_world.init()) {
        cerr << "error opening app at " << IP << ":" << PORT << endl;
        return EXIT_FAILURE;
    } else {
        cout << "app opened successfully at " << IP << ":" << PORT << endl;
    }
    auto * handler = new HelloWorldHandler();
    hello_world.addHandler(handler);
    return hello_world.run();
}