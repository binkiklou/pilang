/*
This file defines every methods required for parsing declarations
*/

#include "syntax.hpp"

#include "../shared/print.hpp"

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

// ident : arr_init
// ident : arrow_expr
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

    if(_p->match(COLON))
    {
        if(get_array_expr()) {}
        else
        {
            _p->error_line_remain("Expected valid array declaration");
            ERROR_EXIT;
        }
    }

    MATCH_EXIT;
}