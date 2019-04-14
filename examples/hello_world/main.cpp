#include <iostream>
#include <akrava/web-server/app.h>

using namespace std;

class HelloWorldHandler : public Handler {
public:
    void exec() override {
        this->getContext()->setResponse(new Response("Hello world"));
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
        cout << "App opened successfully at " << IP << ":" << PORT << endl;
    }
    hello_world.addHandler(new HelloWorldHandler());
    return hello_world.run();
}