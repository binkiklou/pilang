/*
This file defines every methods required for parsing declarations
*/

#include "syntax.hpp"

#include "../shared/print.hpp"

// datatype
// !datatype
bool syntax::get_datatype()
{
    _p->try_hint("datatype");

    _p->match(EXCLAMATION_MARK);
    
    if(!_p->match(DATATYPE))
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// form datatype
bool syntax::get_typespec()
{
    _p->try_hint("typespec");

    if(!_p->match(FORM))
    {
        _p->cancel_try();
        return false;
    }

    if(!_p->match(DATATYPE))
    {
        _p->error_here("Datatype is required after a form.");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;;
}

// 1;2;..
bool syntax::get_arr_size()
{
    _p->try_hint("arr_size");

    if(!get_scalar_expr())
    {
        _p->cancel_try();
        return false;   
    }

    while(_p->match(SEMICOLON))
    {
        if(!get_scalar_expr())
        {
            _p->cancel_try();
            return false;
        }
    }

    _p->keep_hint();
    return true;
}

// identifier
bool syntax::get_arr_copy()
{
    _p->try_hint("arr_copy");

    if(!_p->match(IDENTIFIER))
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// {1,2,..}
bool syntax::get_arr_lit()
{
    _p->try_hint("arr_lit");

    if(!_p->match(LBRACE))
    {
        _p->cancel_try();
        return false;
    }

    if(!get_scalar_expr()){
        _p->cancel_try();
        return false;
    }

    while(_p->match(COMMA))
    {
        if(!get_scalar_expr()){
            _p->error_here("Expected a value");
            _p->cancel_try();
            return false;
        }
    }

    if(!_p->match(RBRACE))
    {
        _p->error_here("Opening brace need closing brace");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// datatype[arr_size]
// datatype[arr_copy]
// datatype[arr_lit]
bool syntax::get_arr_init()
{
    _p->try_hint("arr_init");

    if(!get_datatype())
    {
        _p->cancel_try();
        return false;
    }

    if(!_p->match(LBRACKET)){
        _p->cancel_try();
        return false;
    }

    if(get_arr_size()) {}
    else if(get_arr_copy()) {}
    else if(get_arr_lit()) {}
    else{
        _p->cancel_try();
        return false;
    }

    if(!_p->expect(RBRACKET)){
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// ident : arr_init
// ident : arrow_expr
bool syntax::get_vardecl()
{
    _p->try_hint("vardecl");

    if(!get_typespec())
    {
        _p->cancel_try();
        return false;
    }

    // Expect

    if(!_p->match(IDENTIFIER))
    {
        _p->error_here("An identifier is required after a typespec");
        _p->cancel_try();
        return false;
    }

    if(!_p->match(COLON))
    {
        _p->cancel_try();
        return false;
    }

    if(get_array_expr()) {}
    else
    {
        _p->error_line_remain("Expected valid array declaration");
        _p->cancel_try();
        return false;
    }

    print_verbose("keeping vdecl");

    _p->keep_hint();
    return true;
}