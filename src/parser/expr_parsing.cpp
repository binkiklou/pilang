/*
This file defines every method required for expression parsing.
In the syntax parser, operations are not parsed according to their order. Ordering
of operations is done in the AST builder.
*/

#include "parser/syntax.hpp"

// Const_value, so any literal and nothing else
bool syntax::get_scalar_value()
{
    HINT_START("scalar_value");

    if(
        _p->match(INT_LIT) || 
        _p->match(BOOL_LIT) || 
        _p->match(CHAR_LIT) ||
        _p->match(FLOAT_LIT) ||
        _p->match(DOUBLE_LIT)|| 
        _p->match(IDENTIFIER))
    {
        MATCH_EXIT;
    }

    CANCEL_EXIT;
}

//* Expression could be a single term
bool syntax::get_scalar_expr()
{
    HINT_START("scalar_expr");

    if(get_scalar_value()){}
    else if(get_scalar_operation()){}
    else
    {
        CANCEL_EXIT;
    }

    while(_p->match(MATH_OP))
    {
        if(get_scalar_value()){}
        else if(get_scalar_operation()){}
        else
        {
            _p->error_here("Expected scalar term after operator.");
            ERROR_EXIT;
        }
    }

    MATCH_EXIT;
}

// scalar_operator x
// scalar_operator(x)
bool syntax::get_scalar_operation()
{
    HINT_START("scalar_operation");

    if(!_p->match(SCALAR_OPERATOR))
    {
        CANCEL_EXIT;
    }

    // expect

    bool enclosed = false;

    if(_p->match(LPAREN)) {enclosed = true;}

    if(!_p->match(IDENTIFIER))
    {
        _p->error_here("Expected an identifier after an operator.");
        ERROR_EXIT;
    }

    if(enclosed && !_p->expect(RPAREN))
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// selection
// arr_init
// (arr_expr)
bool syntax::get_scalar_arrow_left()
{
    HINT_START("scalar_arrow_left");
    
    if(get_selection()){}
    else if(get_arr_init()){}
    else if(_p->match(LPAREN))
    {
        if(!get_array_expr())
        {
            CANCEL_EXIT;
        }

        // Expect
        if(!_p->expect(RPAREN)){
            CANCEL_EXIT;
        }
    }
    else
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// scalar_expr
bool syntax::get_scalar_arrow_right()
{
    HINT_START("scalar_arrow_right");

    if(!get_scalar_expr()){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// scalar_arrow_left -> scalar_arrow_right
bool syntax::get_scalar_arrow_expr()
{
    HINT_START("scalar_arrow_expr");

    if(!get_scalar_arrow_left()){
        CANCEL_EXIT;
    }

    if(!_p->match(SCALAR_ARROW)){
        CANCEL_EXIT;
    }

    // Expect

    if(!get_scalar_arrow_right()){
        _p->error_line_remain("Right-side of a scalar arrow must be valid.");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// [arr_shape]
bool syntax::get_array_arrow_right()
{
    HINT_START("array_arrow_right");

    if(!_p->match(LBRACKET)){
        CANCEL_EXIT;
    }

    if(!get_arr_size()){
        CANCEL_EXIT;
    }

    if(!_p->expect(RBRACKET)){
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// (arr_expr)
bool syntax::get_array_arrow_left()
{
    HINT_START("array_arrow_left");

    if(!_p->match(LPAREN)){
        CANCEL_EXIT;
    }

    if(!get_array_expr()){
        CANCEL_EXIT;
    }

    if(!_p->expect(RPAREN)){
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// array_arrow_left => array_arrow_right
bool syntax::get_array_arrow_expr()
{
    HINT_START("array_arrow_expr");

    if(!get_array_arrow_left()){
        CANCEL_EXIT;
    }

    if(!_p->match(RIGHT_BIG_ARROW)){
        CANCEL_EXIT;
    }

    // Expect

    if(!get_array_arrow_right()){
        _p->error_line_remain("Expected a valid right-side for reshape arrow.");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// scalar_value
// {scalar_value}:scalar_value
bool syntax::get_selector()
{
    HINT_START("selector");

    if(get_scalar_value()){}
    else if(_p->match(LBRACE))
    {
        // Expect

        if(!get_scalar_value()){
            CANCEL_EXIT;
        }

        if(!_p->expect(RBRACE)){
            CANCEL_EXIT;
        }

        if(!_p->expect(COLON)){
            CANCEL_EXIT;
        }

        if(!get_scalar_value()){
            CANCEL_EXIT;
        }
    }
    else
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// arr_init(selector)
// (array_expr)(selector)
bool syntax::get_selection()
{
    HINT_START("selection");

    if(_p->match(LPAREN))
    {
        if(!get_selector()){
            CANCEL_EXIT;
        }

        // Expect

        if(!get_array_expr())
        {
            CANCEL_EXIT;
        }

        if(!_p->expect(RPAREN))
        {
            CANCEL_EXIT;
        }
    }

    if(!get_arr_init())
    {
        CANCEL_EXIT;
    }

    if(!_p->match(LPAREN)){
        CANCEL_EXIT;
    }

    // Expect

    if(!get_selector())
    {
        _p->error_here("Selector was expected.");
        ERROR_EXIT;
    }

    if(!_p->expect(RPAREN)){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// Any expression that can return an array
// ---
// reshape_arrow_expr
// scalar_arrow_expr
// (array_expr)
// call
// array_init
bool syntax::get_array_expr_term()
{
    HINT_START("array_expr_term");

    if(!get_array_arrow_expr() && !get_scalar_arrow_expr())
    {
        // Parenthesis creates a sub-expression, if the parenthesis is
        // not part of the scalar-arrow-expression
        if(_p->match(LPAREN)){
            if(!get_array_expr()){
                CANCEL_EXIT;
            }

            if(!_p->match(RPAREN)){
                _p->error_here("Sub expression must be closed");
                ERROR_EXIT;
            }
        }
        else if(get_call()){}
        else if(get_arr_init()){}
        else
        {
            CANCEL_EXIT;
        }
    }

    MATCH_EXIT;
}

// Array expressions are any expression that involve one array,
// it can be a single term, or operation.
// ---
bool syntax::get_array_expr()
{
    HINT_START("array_expr");

    if(!get_array_expr_term()){
        CANCEL_EXIT;
    }

    while(_p->match(MATH_OP)){
        if(!get_array_expr_term()){
            _p->error_here("Expected a valid array term after operation");
            ERROR_EXIT;
        }
    }
    
    MATCH_EXIT;
}