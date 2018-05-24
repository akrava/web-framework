#include <app.h>
#include <runtime_exception.h>
#include <iostream>

App::App(std::string & ip, int port, bool isIPv6, const char *logFilePath) : socket(ip, port, isIPv6) {

    // todo: logging in file- / console- stream
}

App::App(InitParams params) : socket(params) {

    // todo: logging init also
    params.getFilePath();
}



bool App::init() {
    try {
        socket.init();
    } catch (RuntimeException & err) {
        std::cerr << err.what() << std::endl;
        return false;
    }
    return true;
}

void App::addHandler(Handler handler) {

}

void App::addHandler(std::list<Handler> handlerList) {

}

void App::addHandler(std::vector<Handler> handlerList) {

}

void App::run() {
    std::string d;
    while (true) {

        try {
            d = socket.getData();

            std::string ex = "<!doctype html><html><body><center><h1>TEST</h1></center><h2>/</h2><h3>? - ?</h3><p>.,.... </p></body></html>";

            std::string q1("HTTP/1.1 200 OK\r\n");
            socket.reciveData(q1);

            std::string q2("Content-length: 109\r\n");
            socket.reciveData(q2);

            std::string q3("Content-Type: text/html; charset=utf-8\r\n");
            socket.reciveData(q3);

            std::string q4("Connection: close\r\n\r\n");
            socket.reciveData(q4);


            socket.reciveData(ex);
        } catch (RuntimeException & err) {
            std::cerr << err.what() << std::endl;
            break;
        }
        if (d== "exit") break;
    }

}

void App::close() {

}
