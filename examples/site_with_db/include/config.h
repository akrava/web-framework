#pragma once

#include <string>

#ifdef __linux__
#define __PATH_TO_SHARED_FOLDER "./../assets"
#define __PATH_TO_DATA "/../assets"
#define __PATH_TO_TEMPLATES "/../templates"
#elif _WIN32
#define __PATH_TO_DATA "F:\\Projects\\github\\web-framework\\examples\\site_with_db\\assets"
#define __PATH_TO_SHARED_FOLDER __PATH_TO_DATA
#define __PATH_TO_TEMPLATES "F:\\Projects\\github\\web-framework\\examples\\site_with_db\\templates"
#endif
#define __SECRET  "secret"

std::string currentDir();

std::string toString(double val, int precision = 2);

time_t toTime_t(std::string & date, const char * format = "%Y-%m-%d");
