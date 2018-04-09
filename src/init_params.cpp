#include <init_params.h>
#include <iostream>
#include <chrono>
#include <cstring>

const static bool def_ipv6 = false;
const static int def_port = 80;
const static char * def_ip = "127.0.0.1";
const static char * def_log_filename = nullptr;

InitParams::InitParams(int argc, char ** argv) {
    bool default_settings = false;
    if (argc == 1) {
        default_settings = true;
        this->run_app = true;
        this->ipv6 = def_ipv6;
        this->ip = def_ip;
        this->port = def_port;
        if (def_log_filename) {
            std::time_t result = std::time(nullptr);
            this->log_filename = std::ctime(&result);
            this->log_filename += " application logfile.log";
        }
    } else {
        bool help_view = false;
        for (int i = 1; i < argc; i++) {
            if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
                this->run_app = false;
                if (i != 1) {
                    std::cout << "To many params. Try only help." << std::endl
                              << "Example: " << argv[0] << " " << argv[i] << std::endl;
                } else {
                    std::cout << "Usage: " << argv[0] << "ip address (v6/v4) : port" << std::endl
                              << "And some extra: --log [-l] and filename --help [-h]"
                              << "Example: " << argv[0] << " soon..." << std::endl;
                }
                break;
            } else if (!strcmp("--log", argv[i]) || !strcmp("-l", argv[i])) {
                if (i + 1 == argc) {
                    this->run_app = false;
                    std::cout << "err: no filename" << std::endl;
                    break;
                }
                this->log_filename += argv[i + 1];
            } else {
                if (*argv[i] == '[') {
                    this->ipv6 = true;
                    char * tmp = argv[i];
                    tmp++;
                    while (*tmp != ']' && *tmp != 0) {
                        this->ip += *tmp;
                        tmp++;
                    }
                    if (*tmp == 0) abort(); // todo
                    tmp +=2;
                    this->port = (int) strtol(tmp, nullptr,10);
                } else {
                    this->ipv6 = false;
                    char * tmp = argv[i];
                    while (*tmp != ':' && *tmp != 0) {
                        this->ip += *tmp;
                        tmp++;
                    }
                    if (*tmp == 0) abort(); // todo
                    tmp++;
                    this->port = (int) strtol(tmp, nullptr,10);
                }
            }
        }
        this->run_app = this->ip.length() != 0;
    }
    if (this->run_app) {
        std::cout << "Application is running with" << (default_settings ? " default " : " ")
                  << "settings:" << std::endl << "on ip: " << this->ip << " and port: "
                  << this->port << "." << std::endl;
    } else {
        std::cout << "An error occurred, couldn't launch the app." << std::endl;
    }
}

bool InitParams::getIpv6() {
    return this->ipv6;
}

std::string InitParams::getIp() {
    return this->ip;
}

int InitParams::getPort() {
    return this->port;
}
