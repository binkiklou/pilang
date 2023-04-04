#include "token.hpp"

#include <type_traits>

#define tknstr(c) #c,

// Must be in the same order as enum definitions
char const* token_names[] = {
    tknstr(UNKNOWN)
    tknstr(MATH_OP)
    tknstr(LBRACKET)
    tknstr(RBRACKET)
    tknstr(LBRACE)
    tknstr(RBRACE)
    tknstr(LPAREN)
    tknstr(RPAREN)
    tknstr(COLON)
    tknstr(SEMICOLON)
    tknstr(PERIOD)
    tknstr(COMMA)
    
    tknstr(SCALAR_ARROW)
    tknstr(LEFT_BIG_ARROW)
    tknstr(RIGHT_BIG_ARROW)
    tknstr(EQUAL)
    tknstr(GREATER)
    tknstr(SMALLER)

    tknstr(KW_RANGE)
    tknstr(DATATYPE)
    tknstr(IDENTIFIER)
    tknstr(BOOL_LIT)
    tknstr(INT_LIT)
    tknstr(DOUBLE_LIT)
    tknstr(FLOAT_LIT)
    tknstr(CHAR_LIT)
    tknstr(STRING_LIT)
    tknstr(_EOF)
};

//static_assert(std::size(token_names) == static_cast<int>(TKN_TYPE::_EOF) + 1, "Size of token names array doesn't match number of tokens");

std::string tkn_type_as_string(TKN_TYPE t)
{
    std::string result;
    result += token_names[static_cast<int>(t)];
    return result;
}