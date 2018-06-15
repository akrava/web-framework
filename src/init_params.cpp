#include <init_params.h>
#include <iostream>
#include <chrono>
#include <cstring>
#include <fstream>
#include <sstream>
#include <runtime_exception.h>
#include <sys/socket.h>
#include <cerrno>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

namespace {
    const bool def_ipv6 = false;
    const int def_port = 80;
    const char * def_ip = "127.0.0.1";
}

static bool isValidIP(string & host, bool * IPv6);
static bool isValidHostName(string & host);
static bool isValidFilePath(const char * filePath);
static bool isValidIPv4Block(string & block);
static bool isValidIPv6Block(string & block);
static bool isValidHostBlock(string & block);
static string getIpFromDomain(string &domain, bool isHttps, bool *IPv6);

InitParams::InitParams() {
    port = -1;
    logFilePath = string();
    host = string();
}

InitParams::InitParams(int argc, char ** argv) : InitParams() {
    bool defaultSettings = false;
    if (argc == 1) {
        defaultSettings = true;
        IPv6 = def_ipv6;
        host = def_ip;
        port = def_port;
        logFilePath = string();
    } else {
        string errorMsg = "Error while initializing. Try --help";
        for (int i = 1; i < argc; i++) {
            if ((!strcmp("--help", argv[1]) || !strcmp("-h", argv[1])) && argc == 2) {
                cout << "Usage: " << argv[0] << " host address (IPv4/IPv6/domain) : port (default - 80)"
                     << std::endl << "And some extra: --log [-l] and filename OR --help [-h]" << endl
                     << "Hint: IPv6 must be inside [] brackets; domain should start as http://"
                     << std::endl;
                throw RuntimeException("Try again");
            } else if (logFilePath.empty() && (!strcmp("--log", argv[i]) || !strcmp("-l", argv[i]))) {
                if (i + 1 == argc) {
                    throw RuntimeException("Error while initializing: no file path is given");
                }
                string path = string();
                path += argv[i + 1];
                if (!isValidFilePath(path.c_str())) {
                    throw RuntimeException("Error while initializing: file path is incorrect");
                } else {
                    logFilePath = path;
                    i++;
                }
            } else if (host.empty()) {
                string domain = argv[i];
                size_t portPosition = domain.find_last_of(':');
                size_t squareBracketPos = domain.find_last_of(']');
                if (portPosition == string::npos || ((domain.find_first_of("http://") == 0
                    || domain.find_first_of("https://") == 0) && portPosition < 7)
                    || (portPosition < squareBracketPos && squareBracketPos != string::npos)) {
                    port = def_port;
                    portPosition = domain.length();
                }
                if (*argv[i] == '[') {
                    bool correctVersion = false;
                    string IPv6(argv[i], 1, portPosition - 2) ;
                    if (isValidIP(IPv6, &correctVersion)
                        && correctVersion && domain[portPosition - 1] == ']')
                    {
                        host = IPv6;
                        this->IPv6 = true;
                    } else {
                        throw RuntimeException("Error while initializing. Invalid IPv6");
                    }
                } else {
                    bool isIPv6 = true;
                    string IPv4(argv[i], 0, portPosition) ;
                    if (isValidIP(IPv4, &isIPv6) && !isIPv6) {
                        this->IPv6 = false;
                        host = IPv4;
                    } else if (domain.find_first_of("http://") == 0 && domain.length() > 8) {
                        string curDomain = IPv4.substr(8);
                        if (!isValidHostName(curDomain)) throw RuntimeException(errorMsg);
                        bool checkIfIPv6;
                        host = getIpFromDomain(curDomain, false, &checkIfIPv6);
                        IPv6 = checkIfIPv6;
                    } else if (domain.find_first_of("https://") == 0 && domain.length() > 9) {
                        string curDomain = IPv4.substr(9);
                        if (!isValidHostName(curDomain)) throw RuntimeException(errorMsg);
                        bool checkIfIPv6;
                        host = getIpFromDomain(curDomain, true, &checkIfIPv6);
                        IPv6 = checkIfIPv6;
                    } else {
                        throw RuntimeException(errorMsg);
                    }
                }
                if (port < 0) {
                    if (portPosition + 1 >= domain.length()) throw RuntimeException(errorMsg);
                    size_t lastNumPortPos;
                    string errPort("Error while initializing. Invalid port: ");
                    try {
                        port = stoi(domain.substr(portPosition + 1), &lastNumPortPos);
                    } catch (invalid_argument & err) {
                        throw RuntimeException(errPort + err.what());
                    } catch (out_of_range & err) {
                        throw RuntimeException(errPort + err.what());
                    }
                    if (lastNumPortPos != domain.length() - portPosition - 1
                        || port < 0 || port > 65535) throw RuntimeException(errorMsg);
                }
            } else {
                throw RuntimeException(errorMsg);
            }
        }
    }
    cout << "Application is running with" << (defaultSettings ? " default " : " ")
         << "settings:" << endl << "on ip: " << host << " and port: " << port;
    if (logFilePath.empty()) {
        cout << "." << endl;
    } else {
        cout << "\nLog is enabled on file path: " << logFilePath << endl;
    }
}

bool InitParams::isIPv6() {
    return IPv6;
}

const char * InitParams::getIP() {
    return host.c_str();
}

int InitParams::getPort() {
    return port;
}

std::string InitParams::getFilePath() {
    return logFilePath;
}

static bool isValidIP(string & host, bool * IPv6) {
    stringstream IP(host);
    string block;
    if (host.substr(0, 4).find('.') != string::npos) {
        for (int i = 0; i < 4; i++) {
            if (!getline(IP, block, '.') || !isValidIPv4Block(block)) return false;
        }
        *IPv6 = false;
        return IP.eof();
    } else if (host.substr(0, 5).find(':') != string::npos) {
        for (int i = 0; i < 8; i++) {
            if (!getline(IP, block, ':') || !isValidIPv6Block(block)) return false;
        }
        *IPv6 = true;
        return IP.eof();
    }
    return false;
}

static bool isValidHostName(string & host) {
    if (host.length() > 255) return false;
    size_t dotPosition = host.find('.');
    if (dotPosition == string::npos || dotPosition == 0
            || dotPosition == host.length() - 1) return false;
    stringstream hostStream(host);
    string block;
    while (getline(hostStream, block, '.')) {
        if (!isValidHostBlock(block)) return false;
    }
    return isValidHostBlock(block);
}

static bool isValidFilePath(const char * filePath) {
    ofstream file(filePath);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

// http://www.nic.ag/rules.htm
static bool isValidHostBlock(string & block) {
    size_t len = block.length();
    if (len < 1 || len > 63) return false;
    if (block[0] == '-' || block[len - 1] == '-') return false;
    for (char & cur : block) {
        if (!isalnum(cur) && cur != '-') return false;
    }
    return true;
}

// https://en.wikipedia.org/wiki/IPv4#Addressing
static bool isValidIPv4Block(string & block) {
    int num = 0;
    if (!block.empty() && block.size() <= 3) {
        for (size_t i = 0; i < block.size(); i++) {
            char c = block[i];
            if (!isdigit(c) || (i == 0 && c == '0' && block.size() > 1)) {
                return false;
            } else {
                num *= 10;
                num += c - '0';
            }
        }
        return num <= 255;
    }
    return false;
}

// https://en.wikipedia.org/wiki/IPv6#Addressing
static bool isValidIPv6Block(string & block) {
    if (!block.empty() && block.size() <= 4) {
        for (char & c : block) {
            if (!isxdigit(c)) return false;
        }
        return true;
    }
    return false;
}

static string getIpFromDomain(string &domain, bool isHttps, bool *IPv6) {
    addrinfo hints, * serverInfo;
    int error;
    string ip;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((error = getaddrinfo(domain.c_str(), isHttps ? "https" : "http", &hints, &serverInfo)) != 0)  {
        string err = "Error while initializing: Cannot get ip: ";
        err += gai_strerror(error);
        throw RuntimeException(err);
    }
    for (auto * temp = serverInfo; temp != nullptr; temp = temp->ai_next) {
        if (temp->ai_flags == AI_PASSIVE && !temp->ai_next) continue;
        auto *h = (struct sockaddr_in *)temp->ai_addr;
        ip = inet_ntoa(h->sin_addr);
        *IPv6 = h->sin_family == AF_INET6;
        if (temp->ai_next && temp->ai_next->ai_flags == AI_PASSIVE && !temp->ai_next->ai_next) break;
    }
    freeaddrinfo(serverInfo);
    return ip;
}
