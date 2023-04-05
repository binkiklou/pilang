#pragma once

#include <string>

#include "../lexer/source.hpp"

// Location in the compiler
enum diagnostic_type
{
    lexing_diag,
    syntax_diag,
    semantic_diag,
    codegen_diag 
};


enum diagnostic_level
{
    warning,
    error,
    info
};

/*
Error/Warning message, shared accross the compiler
*/
class diagnostic
{
    public:
    diagnostic_type m_type;
    diagnostic_level m_level;
    std::string m_msg;

    bool m_show = false; // Show a view of where the error is
    unsigned int m_vlcount = 0; // number of lines before and after
    bool m_has_cursor = false;
    bool m_cursor_line = false; // if true ignore length and take whole line
    unsigned int m_clength = 0; // cursor length

    location m_loc;
};