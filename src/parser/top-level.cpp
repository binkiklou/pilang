#include "parser.hpp"

#include "syntax.hpp"

#include "../shared/print.hpp"

// a:[3;3]

bool syntax::get_arr_init()
{
    _p->try_hint("arr_init");

    if(!_p->expect(LBRACKET)){
        _p->cancel_try();
        return false;
    }

    if(_p->match(INT_LIT) && _p->match(SEMICOLON) && _p->match(INT_LIT))
    {
        if(!_p->expect(RBRACKET)){
            _p->cancel_try();
            return false;
        }

        _p->keep_hint();
        return true;
    }

    _p->cancel_try();
    return false;
}

bool syntax::get_vardecl()
{
    _p->try_hint("vardecl");

    if(!_p->match(IDENTIFIER) || !_p->match(COLON)){
        _p->cancel_try();
        return false;
    }

    if(!get_arr_init())
    {
        _p->error_here("Declaration expected an array inition");
        _p->cancel_try();
        return false;
    }
    _p->keep_hint();
    return true;
}

void syntax::get_top_level()
{
    if(get_vardecl()){}
    else{
        _p->error_here("Unrecognized top-level statement");
    }
}

bool parser::parse()
{
    if(m_state != PARSER_OK)
    {
        return false;
    }

    print_verbose("Starting parse");

    syntax s(this);

    // Top-Level loop 
    int last_pos = 0;
    int loop_count = 0;
    
    // Error handling not yet implemented
    while(this->_pos < this->tokens->size() && m_state == PARSER_STATE::PARSER_OK)
    {
        s.get_top_level();

        // Loop detection
        if(this->_pos == last_pos){
            loop_count++;
        }
        if(loop_count >= 3){
            this->m_state = PARSER_STATE::PARSER_UNRECOVERABLE;
            error_here("Parser infinite loop, exiting.");
            return false;
        }
    }

    if(m_state != PARSER_STATE::PARSER_OK){
        return false;
    }

    return true;
}