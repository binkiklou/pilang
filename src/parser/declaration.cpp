/*
This file defines every methods required for parsing declarations
*/

#include "parser/syntax.hpp"

#include "shared/print.hpp"

// datatype
// !datatype
bool syntax::get_datatype()
{
    HINT_START("datatype");

    _p->match(EXCLAMATION_MARK);
    
    if(!_p->match(DATATYPE))
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// form datatype
bool syntax::get_typespec()
{
    HINT_START("typespec");

    if(!_p->match(FORM))
    {
        CANCEL_EXIT;
    }

    if(!_p->match(DATATYPE))
    {
        _p->error_here("Datatype is required after a form.");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// 1;2;..
bool syntax::get_arr_size()
{
    HINT_START("arr_size");

    if(!get_scalar_expr())
    {
        CANCEL_EXIT;
    }

    while(_p->match(SEMICOLON))
    {
        if(!get_scalar_expr())
        {
            CANCEL_EXIT;
        }
    }

    MATCH_EXIT;
}

// identifier
bool syntax::get_arr_copy()
{
    HINT_START("arr_copy");

    if(!_p->match(IDENTIFIER))
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// {1,2,..}
bool syntax::get_arr_lit()
{
    HINT_START("arr_lit");

    if(!_p->match(LBRACE))
    {
        CANCEL_EXIT;
    }

    if(!get_scalar_expr()){
        CANCEL_EXIT;
    }

    while(_p->match(COMMA))
    {
        if(!get_scalar_expr()){
            _p->error_here("Expected a value");
            ERROR_EXIT;
        }
    }

    if(!_p->match(RBRACE))
    {
        _p->error_here("Opening brace need closing brace");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// datatype[arr_size]
// datatype[arr_copy]
// datatype[arr_lit]
bool syntax::get_arr_init()
{
    HINT_START("arr_init");

    if(!get_datatype())
    {
        CANCEL_EXIT;
    }

    if(!_p->match(LBRACKET)){
        CANCEL_EXIT;
    }

    if(get_arr_size()) {}
    else if(get_arr_copy()) {}
    else if(get_arr_lit()) {}
    else{
        CANCEL_EXIT;
    }

    if(!_p->expect(RBRACKET)){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// typespec ident
bool syntax::get_param()
{
    HINT_START("param");

    if(!get_typespec()){
        CANCEL_EXIT;
    }

    if(!_p->match(IDENTIFIER)){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// (param, param, ...)
bool syntax::get_param_list()
{
    HINT_START("param_list");

    if(!_p->match(LPAREN)){
        CANCEL_EXIT;
    }

    if(get_param()){
        while(_p->match(COMMA))
        {
            if(!get_param()){
                _p->error_here("A parameter must be placed after a comma.");
                ERROR_EXIT;
            }
        }
    }

    if(!_p->match(RPAREN)){
        CANCEL_EXIT;
    }


    MATCH_EXIT;
}

// fn typespec identifier param_list : array_expr
bool syntax::get_fndecl()
{
    HINT_START("fndecl");

    if(!_p->match(KW_FN)){
        CANCEL_EXIT;
    }

    // expect

    if(!get_typespec()){
        _p->error_here("Typespecification is expected after the fn Keyword.");
        ERROR_EXIT;
    }

    if(!_p->match(IDENTIFIER)){
        _p->error_here("Function declaration need an identifier.");
        ERROR_EXIT;
    }

    if(!get_param_list()){
        _p->error_here("Function Declaration needs parameters");
        ERROR_EXIT;
    }

    if(!_p->expect(COLON)){
        ERROR_EXIT
    }
    
    if(!get_array_expr()){
        _p->error_here("Function Declaration needs an expression");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// ident : arr_init
// ident : arrow_expr
bool syntax::get_vardecldef()
{
    HINT_START("vardecldef");

    if(!get_typespec())
    {
        CANCEL_EXIT;
    }

    if(!_p->match(IDENTIFIER))
    {
        _p->error_here("An identifier is required after a typespec");
        ERROR_EXIT;
    }

    if(!_p->match(COLON))
    {
        CANCEL_EXIT;
    }

    if(get_array_expr()) {}
    else
    {
        _p->error_line_remain("Expected valid array declaration");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

bool syntax::get_vardecl()
{
    HINT_START("vardecl");

    if(!get_typespec())
    {
        CANCEL_EXIT;
    }

    if(!_p->match(IDENTIFIER))
    {
        _p->error_here("An identifier is required after a typespec");
        ERROR_EXIT;
    }


    MATCH_EXIT;
}