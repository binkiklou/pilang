#pragma once

#include "lexer/source.hpp"

enum TKN_TYPE {
    UNKNOWN,

    // Symbols
    MATH_OP, PERCENT, DOLLAR, AT,
    LBRACKET, RBRACKET, LBRACE, RBRACE, LPAREN, RPAREN,
    COLON, SEMICOLON, PERIOD, COMMA, EXCLAMATION_MARK, QUESTION_MARK,
    SCALAR_ARROW, LEFT_BIG_ARROW, RIGHT_BIG_ARROW,
    EQUAL, GREATER, SMALLER,

    // Keywords
    KW_ENTRY, KW_PROC, KW_FN,

    SCALAR_OPERATOR, FORM, // arr scalar
    DATATYPE, IDENTIFIER,

    // Literals
    BOOL_LIT, INT_LIT, DOUBLE_LIT, FLOAT_LIT, CHAR_LIT, STRING_LIT,

    _EOF
};

// god that name is bad
std::string tkn_type_as_string(TKN_TYPE);

class token
{
    public:
    // Position of token in the token vector
    unsigned int m_id;

    TKN_TYPE m_type;
    word m_word;
};