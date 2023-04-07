#include "syntax.hpp"

// Single argument
// const_value
bool syntax::get_call_arg()
{
    HINT_START("call_arg");

    // Temporary
    if(_p->match(STRING_LIT)){}
    else if(_p->match(CHAR_LIT)){}
    else if(get_scalar_value()){}
    else
    {
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

// (call_arg,call_arg,..)
bool syntax::get_call_arg_list()
{
    HINT_START("call_arg_list");

    if(!_p->match(LPAREN)){
        CANCEL_EXIT;
    }

    if(!get_call_arg()){
        CANCEL_EXIT;
    }

    while(_p->match(COMMA))
    {
        if(!get_call_arg()){
            _p->error_here("An argument must be placed after a comma.");
            ERROR_EXIT;
        }
    }

    if(!_p->match(RPAREN)){
        CANCEL_EXIT;
    }

    MATCH_EXIT;
}

bool syntax::get_proc_call()
{
    HINT_START("proc_call");

    if(!_p->match(PERCENT))
    {
        CANCEL_EXIT;
    }

    // Expect

    if(!_p->expect(IDENTIFIER)){
        ERROR_EXIT;
    }

    if(!get_call_arg_list())
    {
        _p->error_line_remain("Procedure call requires arguments.");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}