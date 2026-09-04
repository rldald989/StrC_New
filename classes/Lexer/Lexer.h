#pragma once

#include <vector>
#include <string>
#include <bit>
#include <cstring>

#include "../FSViewer.h"

#include "../Util/Util.h"

enum variable_type{
    _NULL,
    _INT,
    _CHAR,
    _BOOL,
    _FLOAT,
    _STRING
};

struct variable{
    const char* name;
    char* value;
    variable_type type;

    variable() {}
    variable(const char* _name, char* _value, variable_type _type): name(_name), value(_value), type(_type) {}
    ~variable() { }

    void clear();

    bool operator==(const std::string& other){ return (name == other); }
};

class Lexer
{
public:
    Lexer(std::vector<std::string*> tokens);
    ~Lexer();

    void read_lines();

private:

    static void replace_all(std::string& buffer, std::string to_replace, std::string replace_str);
    static std::string consume_string(file_viewer& fv, std::string& buffer);

    std::vector<std::string*> m_tokens;
    std::vector<variable> m_variables;
};