#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

static char punctuation_tokens[] =
{
    '`',
    '~',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '-',
    '_',
    '=',
    '+',
    '[',
    ']',
    '{',
    '}',
    '\\',
    ';',
    ':',
    '\'',
    '\"',
    ',',
    '.',
    '<',
    '>',
    '?',
    '/',
};

class Tokenizer
{
public:
    Tokenizer(std::string file_name);
    ~Tokenizer();

    std::vector<std::string*> tokenize();
private:
    std::ifstream m_in;
    std::string m_code;

    int m_position;

    char peek(int ahead = 1) const;
    
    char consume();
};