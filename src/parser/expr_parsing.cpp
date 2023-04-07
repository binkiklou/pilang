/*
This file defines every method required for expression parsing.
In the syntax parser, operations are not parsed according to their order. Ordering
of operations is done in the AST builder.
*/

#include "syntax.hpp"

// Const_value, so any literal and nothing else
bool syntax::get_scalar_value()
{
    _p->try_hint("scalar_value");

    if(
        _p->match(INT_LIT) || 
        _p->match(BOOL_LIT) || 
        _p->match(CHAR_LIT) ||
        _p->match(FLOAT_LIT) ||
        _p->match(DOUBLE_LIT)|| 
        _p->match(IDENTIFIER))
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
    _p->try_hint("scalar_expr");

    if(get_scalar_value()){}
    else if(get_scalar_operation()){}
    else
    {
        _p->cancel_try();
        return false;
    }

    while(_p->match(MATH_OP))
    {
        if(get_scalar_value()){}
        else if(get_scalar_operation()){}
        else
        {
            _p->error_here("Expected scalar term after operator.");
            _p->cancel_try();
            return false;
        }
    }

    _p->keep_hint();
    return true;
}

// scalar_operator x
// scalar_operator(x)
bool syntax::get_scalar_operation()
{
    _p->try_hint("scalar_operation");

    if(!_p->match(SCALAR_OPERATOR))
    {
        _p->cancel_try();
        return false;
    }

    // expect

    bool enclosed = false;

    if(_p->match(LPAREN)) {enclosed = true;}

    if(!_p->match(IDENTIFIER))
    {
        _p->error_here("Expected an identifier after an operator.");
        _p->cancel_try();
        return false;
    }

    if(enclosed && !_p->expect(RPAREN))
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// selection
// arr_init
// (arr_expr)
bool syntax::get_scalar_arrow_left()
{
    _p->try_hint("scalar_arrow_left");
    
    if(get_selection()){}
    else if(get_arr_init()){}
    else if(_p->match(LPAREN))
    {
        if(!get_array_expr())
        {
            _p->cancel_try();
            return false;
        }

        // Expect
        if(!_p->expect(RPAREN)){
            _p->cancel_try();
            return false;
        }
    }
    else
    {
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
    _p->try_hint("scalar_arrow_expr");

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

// scalar_value
// {scalar_value}:scalar_value
bool syntax::get_selector()
{
    _p->try_hint("selector");

    if(get_scalar_value()){}
    else if(_p->match(LBRACE))
    {
        // Expect

        if(!get_scalar_value()){
            _p->cancel_try();
            return false;
        }

        if(!_p->expect(RBRACE)){
            _p->cancel_try();
            return false;
        }

        if(!_p->expect(COLON)){
            _p->cancel_try();
            return false;
        }

        if(!get_scalar_value()){
            _p->cancel_try();
            return false;
        }
    }
    else
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// arr_init(selector)
// (array_expr)(selector)
bool syntax::get_selection()
{
    _p->try_hint("selection");

    if(_p->match(LPAREN))
    {
        if(!get_selector()){
            _p->cancel_try();
            return false;
        }

        // Expect

        if(!get_array_expr())
        {
            _p->cancel_try();
            return false;
        }

        if(!_p->expect(RPAREN))
        {
            _p->cancel_try();
            return false;
        }
    }

    if(!get_arr_init())
    {
        _p->cancel_try();
        return false;
    }

    if(!_p->match(LPAREN)){
        _p->cancel_try();
        return false;
    }

    // Expect

    if(!get_selector())
    {
        _p->error_here("Selector was expected.");
        _p->cancel_try();
        return false;
    }

    if(!_p->expect(RPAREN)){
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// Any expression that can return an array
// ---
// reshape_arrow_expr
// scalar_arrow_expr
// (array_expr)
// array_select
// array_init
bool syntax::get_array_expr_term()
{
    _p->try_hint("array_expr_term");

    if(!get_scalar_arrow_expr())
    {
        // Parenthesis creates a sub-expression, if the parenthesis is
        // not part of the scalar-arrow-expression
        if(_p->match(LPAREN)){
            if(!get_array_expr()){
                _p->cancel_try();
                return false;
            }

            if(!_p->match(RPAREN)){
                _p->error_here("Sub expression must be closed");
                _p->cancel_try();
                return false;
            }
        }
        else if(get_arr_init()){}
        else
        {
            _p->cancel_try();
            return false;
        }
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
    
    _p->keep_hint();
    return true;
}