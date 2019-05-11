#pragma once

#include <string>

#ifdef __linux__
#define __PATH_TO_DATA "./../assets"
#define __PATH_SEPARATOR "/"
#define __PATH_TO_TEMPLATES "./../templates/"
#define __PATH_TO_DB "./../db/db_file"
#elif _WIN32
#define __PATH_TO_DATA "F:\\Projects\\github\\web-framework\\examples\\site_with_db\\assets"
#define __PATH_SEPARATOR "\\"
#define __PATH_TO_TEMPLATES "F:\\Projects\\github\\web-framework\\examples\\site_with_db\\templates\\"
#define __PATH_TO_DB "F:\\Projects\\github\\web-framework\\examples\\site_with_db\\db\\db_file"
#endif
#define __SECRET  "secret"

std::string currentDir();

std::string toString(double val, int precision = 2);

time_t toTime_t(std::string & date, const char * format = "%Y-%m-%d");
