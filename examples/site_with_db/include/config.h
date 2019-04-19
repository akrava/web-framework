#pragma once

#include <string>

#define __PATH_TO_DATA "/../assets"
#define __PATH_TO_TEMPLATES "/../templates"
#define __SECRET  "secret"

std::string currentDir();

std::string toString(double val, int precision = 2);

time_t toTime_t(std::string & date, const char * format = "%Y-%m-%d");
