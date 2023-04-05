/*
This file defines every method required for expression parsing.
In the syntax parser, operations are not parsed according to their order. Ordering
of operations is done in the AST builder.
*/

#include "syntax.hpp"

// Const_value, so any literal and nothing else
bool syntax::get_const_value()
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
bool syntax::get_const_expr()
{
    _p->try_hint("const_expr");

    if(!get_const_value())
    {
        _p->cancel_try();
        return false;
    }

    while(_p->match(MATH_OP))
    {
        if(!get_const_value())
        {
            _p->error_here("Expected term after operator.");
        }
    }

    _p->keep_hint();
    return true;
}