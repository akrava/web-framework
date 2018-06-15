#include <log_manager.h>
#include <parser_http.h>

LogManager::LogManager(const char * fileName) {
    if (!fileName) {
        isOpened = false;
        return;
    }
    std::string file_str{fileName};
    this->fileName = file_str.append(ParserHTTP::getTime(nullptr, "%d-%m-%Y_%H-%M-%S") + ".log");
    std::ofstream logFile(fileName);
    if (logFile.is_open()) {
        isOpened = true;
        logFile.close();
    } else {
        isOpened = false;
    }
}

void LogManager::operator<<(const char * data) {
    if (!isOpened) return;
    std::ofstream logFile(fileName, std::ios::out | std::ios::app );
    if (logFile.is_open()) {
        logFile << ParserHTTP::getTime(nullptr, "[%d-%m-%Y %H:%M:%S] > ");
        logFile << data << std::endl;
        logFile.close();
    } else {
        isOpened = false;
    }
}

void LogManager::operator<<(std::string data) {
    operator<<(data.c_str());
}
