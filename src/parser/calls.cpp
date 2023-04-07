#include "syntax.hpp"

// Single argument
// const_value
bool syntax::get_call_arg()
{
    _p->try_hint("call_arg");

    // Temporary
    if(_p->match(STRING_LIT)){}
    else if(_p->match(CHAR_LIT)){}
    else if(get_scalar_value()){}
    else
    {
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

// (call_arg,call_arg,..)
bool syntax::get_call_arg_list()
{
    _p->try_hint("call_arg_list");

    if(!_p->match(LPAREN)){
        _p->cancel_try();
        return false;
    }

    if(!get_call_arg()){
        _p->cancel_try();
        return false;
    }

    while(_p->match(COMMA))
    {
        if(!get_call_arg()){
            _p->error_here("An argument must be placed after a comma.");
            _p->cancel_try();
            return false;
        }
    }

    if(!_p->match(RPAREN)){
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

bool syntax::get_proc_call()
{
    _p->try_hint("proc_call");

    if(!_p->match(PERCENT))
    {
        _p->cancel_try();
        return false;
    }

    // Expect

    if(!_p->expect(IDENTIFIER)){
        _p->cancel_try();
        return false;
    }

    if(!get_call_arg_list())
    {
        _p->error_line_remain("Procedure call requires arguments.");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}