#include "Lexer.h"

Lexer::Lexer(std::vector<std::string*> tokens) : m_tokens(tokens)
{
}

Lexer::~Lexer()
{
}

void Lexer::read_lines()
{
    std::vector<std::string*> line;
    file_viewer global_fv(m_tokens);
    
    while(global_fv.peek() != "\0"){
        while(global_fv.peek() != ";"){
            line.push_back(new std::string(global_fv.consume()));
        }

        file_viewer fv(line);

        std::string buffer;

        // TODO : fix bad_alloc when defining multiple strings

        while(fv.position < line.size()){
            if(fv.peek() == "var"){
                variable v;
                fv.forward();

                if(fv.peek() == "<"){
                    fv.forward();
                    while(fv.peek() != ">"){
                        buffer += fv.consume();
                    }

                    if(buffer == "int"){
                        v.type = variable_type::_INT;
                    }
                    else if(buffer == "char"){
                        v.type = variable_type::_CHAR;
                    }
                    else if(buffer == "bool"){
                        v.type = variable_type::_BOOL;
                    }
                    else if(buffer == "float"){
                        v.type = variable_type::_FLOAT;
                    }
                    else if(buffer == "string"){
                        v.type = variable_type::_STRING;
                    }

                    fv.forward();
                    buffer.clear();
                    v.name = fv.consume().c_str();
                }
                else{
                    v.name = fv.consume().c_str();
                }


                if(fv.peek() != ":"){
                    std::cout << "WARNING: Variable " << v.name << " uninitialized." << std::endl;
                }
                else{
                    fv.forward();
                    if(fv.peek() == "\"")
                    {
                        v.type = variable_type::_STRING;

                        fv.forward();

                        buffer = consume_string(fv, buffer);
                        
                        //fv.forward();

                        for(auto& m : m_variables)
                        {
                            std::cout << m.name << std::endl;
                            if(fv.peek() == m.name && m.type == variable_type::_STRING){
                                //variable_found = true;
                                //std::cout << "true" << std::endl;
                                //buffer += reinterpret_cast<const char*>(m.value);
                                //fv.forward();
                                //break;
                            }
                        }

                        if(buffer.size() > 0){
                            char value[buffer.size()];

                            strcpy(value, buffer.c_str());

                            //std::cout << v.name << std::endl;
                            v.value = value;
                            m_variables.push_back(variable(v));
                            buffer.clear();
                        }
                    }
                    else if(is_num(fv.peek()) || fv.peek() == "-")
                    {
                        v.type = variable_type::_INT;
                        int num_val;
                        try
                        {
                            if(fv.peek(0) == "-"){

                                fv.forward();
                                num_val = -std::stoi(fv.consume());
                            }
                            else{
                                num_val = std::stoi(fv.consume());
                            }

                        }
                        catch(const std::out_of_range& e)
                        {
                            std::cout << "ERROR: Integer overflow!" << std::endl;
                            exit(EXIT_FAILURE);
                        }

                        char value[sizeof(num_val)];
                        memcpy(value, &num_val, sizeof(num_val));

                        v.value = value;
                        m_variables.push_back(v);
                        buffer.clear();
                    }
                    else if(fv.peek() == "\'")
                    {
                        v.type = variable_type::_CHAR;
                        fv.forward();
                        while(fv.peek() != "\'"){
                            buffer += fv.consume();
                        }
                        if(buffer.size() == 1){
                            char value[buffer.size()];

                            memcpy(value, buffer.data(), buffer.size());

                            v.value = value;
                        }
                        buffer.clear();
                    }
                }

                if(v.value != nullptr){
                    if(v.type == variable_type::_STRING){
                    std::cout << v.name << ": " << reinterpret_cast<const char*>(v.value) << std::endl;
                    }
                    else if(v.type == variable_type::_INT){
                        std::cout << v.name << ": " << *std::bit_cast<int*>(v.value) << std::endl;
                    }
                    else if(v.type == variable_type::_CHAR){
                        std::cout << v.name << ": " << std::bit_cast<char>(*v.value) << std::endl;
                    }
                }
            }


            fv.forward();
        }

        for(auto& l : line){
            delete l;
        }
        line.clear();

        global_fv.forward();
   }

}



/* A replace method that replaces all instances of a substring within a string. */
void Lexer::replace_all(std::string& buffer, std::string to_replace, std::string replace_str)
{
    /*
        (iteratively)
        A sample is taken of the (buffer) string for later comparison.

        It then compares the substring to find with the sample taken
        to determine if the string should be replaced.

        If replaced, the (itr) position is increased by the replacement
        string's size to accomodate for the (buffer) string's resize.
        The (itr) position is increased iteratively otherwise.
    */ 
    int itr = 0;
    while(itr + to_replace.size() <= buffer.size()){
        std::string current_str = buffer.substr(itr, to_replace.size());
        
        if(current_str == to_replace){
            buffer.replace(itr, to_replace.size(), replace_str);
            itr += replace_str.size();
        }
        else{
            itr++;
        }
    }
}

std::string Lexer::consume_string(file_viewer& fv, std::string& buffer)
{
    std::string built_string;
    
    while(fv.peek() != "\""){
        buffer += fv.consume();
    }

    replace_all(buffer, "\\'", "\'");
    replace_all(buffer, "\\\"", "\"");
    replace_all(buffer, "\\n", "\n");
    replace_all(buffer, "\\b", "\b");
    replace_all(buffer, "\\\\", "\\");

    buffer += "";

    built_string = buffer;
    return built_string;
}
