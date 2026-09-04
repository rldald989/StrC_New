#include "Tokenizer.h"

Tokenizer::Tokenizer(std::string file_name) : m_position(0)
{
    m_in.open(file_name);
    std::string temp = "";
    while(std::getline(m_in, temp))
    {
        m_code += temp;
    }
    m_in.close();
}

Tokenizer::~Tokenizer()
{
    
}

std::vector<std::string*> Tokenizer::tokenize()
{
    std::vector<std::string*> tokens;

    std::string buffer;
    bool quote_flag = false;
    while (peek() != '\0') {
        if (std::isalpha(peek()))
        {
            buffer.push_back(consume());
            while (peek() != '\0' && (std::isalpha(peek()) || peek() == '_'))
            {
                buffer.push_back(consume());
            }

            tokens.push_back(new std::string(buffer));

            buffer.clear();

        }
        else if (std::isdigit(peek()))
        {
            buffer.push_back(consume());
            while (peek() != '\0' && (std::isdigit(peek()) || peek() == '.'))
            {
                buffer.push_back(consume());
            }

            tokens.push_back(new std::string(buffer));

            buffer.clear();

        }
        else if (std::ispunct(peek()))
        {
            buffer.push_back(consume());

            for (int i = 0; i < sizeof(punctuation_tokens) / sizeof(punctuation_tokens[0]); i++) {
                if (buffer[0] == punctuation_tokens[i]) {
                    tokens.push_back(new std::string(buffer));
                    if (*tokens.back() == "\"") {
                        quote_flag = !quote_flag;
                    }
                    buffer.clear();
                }
            }

        }
        else if (std::isspace(peek()))
        {
            buffer.push_back(consume());
            if (quote_flag) {
                tokens.push_back(new std::string(buffer));
            }
            buffer.clear();
        }
        else if (peek() == '\n') {
            consume();
        }
    }
	return tokens;
}

char Tokenizer::peek(int ahead) const
{
    if (m_position + ahead > m_code.length()) {
        return '\0';
    }
    else
    {
        return m_code[m_position];
    }
}

char Tokenizer::consume()
{
    return m_code[m_position++];
}