#pragma once

#include <vector>
#include <string>

enum logger_code{
    _IO,
    _INFO,
    _WARNING,
    _ERROR
};

struct log_data{
    std::string log_message;
    logger_code code;
};

class Logger
{
public:
    Logger(const char* logger_name);
    ~Logger();

private:
    const char* m_logger_name;
    bool m_file_toggle;

    std::vector<log_data> logger_data;
};
