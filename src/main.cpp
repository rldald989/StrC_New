#include "../classes/Tokenizer/Tokenizer.h"
#include "../classes/Lexer/Lexer.h"


int main()
{
    Tokenizer tokenizer("strc/test.strc");

    std::vector<std::string*> tokens = tokenizer.tokenize();

    Lexer lexer(tokens);
    lexer.read_lines();

    for(auto& t : tokens){
        delete t;
    }

    return 0;
}