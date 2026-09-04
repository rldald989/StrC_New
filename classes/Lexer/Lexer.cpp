#include "Lexer.h"

Lexer::Lexer(std::vector<std::string*> tokens) : m_tokens(tokens)
{
}

Lexer::~Lexer()
{
}

void Lexer::read_lines()
{
    // The line vector stores each token before ; is found so the line can be interpreted
    std::vector<std::string*> line;
    // The file_viewer class just allows us to iteratively go through our tokens for analysis
    file_viewer global_fv(m_tokens);
    
    // The loop that searches through our code and currently saves variables
    while(global_fv.peek() != "\0"){
        // Getting everything in the current line
        while(global_fv.peek() != ";"){
            line.push_back(new std::string(global_fv.consume()));
        }

        // Creating a new file viewer to look through the line
        file_viewer fv(line);

        std::string buffer;

        // TODO : fix bad_alloc when defining multiple strings

        // Looking through the line
        while(fv.position < line.size()){
            // If var is found, start looking for other things
            if(fv.peek() == "var"){
                // Temporary variable to store our variable
                variable v;

                // .forward() just allows us to go to the next token
                fv.forward();

                // Looking for if the type is explicitly stated
                if(fv.peek() == "<"){
                    fv.forward();
                    // Getting all the text in that <> and storing it in the buffer string to check the type
                    while(fv.peek() != ">"){
                        buffer += fv.consume();
                    }

                    // Checking and assigning types
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

                    // Cleanup and assigning the name
                    fv.forward();
                    buffer.clear();
                    // .consume() just gets the current token and goes to the next
                    v.name = fv.consume().c_str();
                }
                else{
                    // Stores the name if there is no explicitly stated type
                    v.name = fv.consume().c_str();
                }

                // Looking for initialization
                if(fv.peek() != ":"){
                    std::cout << "WARNING: Variable " << v.name << " uninitialized." << std::endl;
                }
                else{
                    fv.forward();
                    // Looking for and storing a string
                    if(fv.peek() == "\"")
                    {
                        v.type = variable_type::_STRING;

                        fv.forward();

                        // Storing the string
                        buffer = consume_string(fv, buffer);

                        // For now the list of variables is printed each time a new one is found and loaded
                        for(auto& m : m_variables)
                        {
                            std::cout << m.name << std::endl;
                        }

                        // End block where things get made, stored, and cleaned up
                        /*
                            Likely where bad_alloc is originating, it could be because
                            of the value, it may need to be deleted, but I believe I looked
                            it up and objects only need to be deleted if the new keyword
                            is used.

                            It could also be an issue with the vector.
                        */

                        if(buffer.size() > 0){
                            char value[buffer.size()];

                            strcpy(value, buffer.c_str());

                            v.value = value;
                            m_variables.push_back(variable(v));
                            buffer.clear();
                        }
                    }
                    else if(is_num(fv.peek()) || fv.peek() == "-")
                    {
                        // Same sort of setup as the string, except we check to see if there is a '-' token
                        v.type = variable_type::_INT;
                        int num_val;

                        // Using a try catch statement so c++'s std::stoi won't spew out it's own error
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
                            // I exit here, but eventually I'll store errors in a log and print them out and then exit
                            std::cout << "ERROR: Integer overflow!" << std::endl;
                            exit(EXIT_FAILURE);
                        }

                        // Using memcpy here, but I tested without making an int and the bad_alloc issue remained
                        char value[sizeof(num_val)];
                        memcpy(value, &num_val, sizeof(num_val));

                        v.value = value;
                        m_variables.push_back(v);
                        buffer.clear();
                    }
                    else if(fv.peek() == "\'")
                    {
                        // The char is pretty self explanatory considering the previous two
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

                // Spews out the converted value as long as it's not nullptr
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
            else{
                fv.forward();
            }
        }

        // Deleting the tokens in line as they are pointers made using the new keyword
        for(auto& l : line){
            delete l;
        }

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
