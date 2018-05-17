#pragma once
#include <exception>
#include <string>

class RuntimeException : public std::exception {
    std::string _error;
public:
    explicit RuntimeException(const std::string & error) {
        _error = error;
    }
    const char * what() const noexcept override {
        return _error.c_str();
    }
};
