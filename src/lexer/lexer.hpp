#pragma once

#include "source.hpp"

#include "token.hpp"

class lexer
{
    public:
    lexer();
    ~lexer();

    source* m_src;
    
    // Is a pointer in case we need to access it while the lexer is deleted,
    // although deleting the lexer currently also deletes this. But this
    // is easily changed.
    std::vector<token>* tokens = nullptr;
    
    bool tokenize();
    
    // Setting it to false will fail the lexer process
    bool _good = true;
    private:
    std::string _content;
    
    unsigned int _position = 0;

    unsigned int _col = 1;
    unsigned int _line = 1; // Lines starts at one for some reason

    // Creates and outputs the error
    void _make_error(const std::string&);

    void _preprocess();

    void _add_token(token&);
    location _get_loc();

    void _advance();
    void _next();

    bool _skip_space();
    bool _get_symbol();
    bool _get_char_lit();
    bool _get_string_lit();
    bool _get_number();
    // Not to be confused with a source word, this is any text
    bool _get_word();
};