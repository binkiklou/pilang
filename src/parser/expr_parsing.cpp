/*
This file defines every method required for expression parsing.
In the syntax parser, operations are not parsed according to their order. Ordering
of operations is done in the AST builder.
*/

#include "syntax.hpp"

// Const_value, so any literal and nothing else
bool syntax::get_scalar_value()
{
    _p->try_hint("const_value");

    if(
        _p->match(INT_LIT) || 
        _p->match(BOOL_LIT) || 
        _p->match(CHAR_LIT) ||
        _p->match(FLOAT_LIT) ||
        _p->match(DOUBLE_LIT))
    {
        _p->keep_hint();
        return true;
    }

    _p->cancel_try();
    return false;
}

//* Expression could be a single term
bool syntax::get_scalar_expr()
{
    _p->try_hint("const_expr");

    if(!get_scalar_value())
    {
        _p->cancel_try();
        return false;
    }

    while(_p->match(MATH_OP))
    {
        if(!get_scalar_value())
        {
            _p->error_here("Expected term after operator.");
            _p->cancel_try();
            return false;
        }
    }

    _p->keep_hint();
    return true;
}

// array_expr
bool syntax::get_scalar_arrow_left()
{
    _p->try_hint("scalar_arrow_left");

    if(!get_array_expr()){
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// scalar_expr
bool syntax::get_scalar_arrow_right()
{
    _p->try_hint("scalar_arrow_right");

    if(!get_scalar_expr()){
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// scalar_arrow_left -> scalar_arrow_right
bool syntax::get_scalar_arrow_expr()
{
    _p->try_hint("scalar_expr");

    if(!get_scalar_arrow_left()){
        _p->cancel_try();
        return false;
    }

    if(!_p->match(SCALAR_ARROW)){
        _p->cancel_try();
        return false;
    }

    // Expect

    if(!get_scalar_arrow_right()){
        _p->error_line_remain("Right-side of a scalar arrow must be valid.");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// Any expression that must return an array
// ---
// reshape_arrow_expr
// scalar_arrow_expr
// array_select
// array_init
bool syntax::get_array_expr_term()
{
    _p->try_hint("array_expr_term");

    if(get_scalar_arrow_expr()){}
    else if(get_arr_init()){}
    else
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// Array expressions are any expression that involve one array,
// it can be a single term, or operation.
// ---
bool syntax::get_array_expr()
{
    _p->try_hint("array_expr");

    bool enclosed = false;

    if(_p->match(LPAREN))
    {
        enclosed =  true;
    }

    if(!get_array_expr_term()){
        _p->cancel_try();
        return false;
    }

    while(_p->match(MATH_OP)){
        if(!get_array_expr_term()){
            _p->error_here("Expected a valid array term after operation");
            _p->cancel_try();
            return false;
        }
    }

    if(enclosed && !_p->match(RPAREN))
    {
        _p->error_here("Enclosed array expression must be closed.");
        _p->cancel_try();
        return  false;
    }
    
    _p->keep_hint();
    return true;
}