#include <config.h>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#ifdef _WIN32
    #include <windows.h>
#endif

using namespace std;

string currentDir() {
    char cwd[1024];
    #ifdef __linux__
        if (getcwd(cwd, sizeof(cwd)) != nullptr)
            return string(cwd);
    #else
        if (GetModuleFileName(nullptr, cwd, sizeof(cwd)) != nullptr)
            return string(cwd);
    #endif
    return ".";
}

string toString(double val, int precision) {
    stringstream stream;
    stream << fixed << setprecision(precision) << val;
    return stream.str();
}

time_t toTime_t(string & date, const char * format) {
    #ifdef __linux__
        tm tm{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr};
    #else
        tm tm{ 0, 0, 0, 0, 0, 0, 0, 0, 0};
    #endif
    istringstream ss(date);
    ss >> get_time(&tm, format);
    time_t time = mktime(&tm);
    return time;
}
