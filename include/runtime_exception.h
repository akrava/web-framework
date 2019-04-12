#pragma once

#include <exception>
#include <string>

/**
 * @brief exception class for program errors
 *
 * Inherited class from std::exception
 */
class RuntimeException : public std::exception {
    std::string _error;
public:
    /**
     * create RuntimeException with error explanation
     *
     * @param error
     *      explanation of thrown error as std::string
     */
    explicit RuntimeException(const std::string & error) {
        _error = error;
    }

    /**
     * create RuntimeException with error explanation
     *
     * @param error
     *      explanation of thrown error as const char *
     */
    explicit RuntimeException(const char * error) {
        _error = error;
    }

    /**
     * get error information
     *
     * @return
     *      error info as string
     */
    const char * what() const noexcept override {
        return _error.c_str();
    }
};
