#include "parser.hpp"

#include "syntax.hpp"

#include "../shared/print.hpp"

// 1;2;..
bool syntax::get_arr_size()
{
    _p->try_hint("arr_size");

    if(!_p->match(INT_LIT))
    {
        _p->cancel_try();
        return false;   
    }

    while(_p->match(SEMICOLON))
    {
        if(!_p->expect(INT_LIT))
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

    if(!get_value()){
        _p->cancel_try();
        return false;
    }

    while(_p->match(COMMA))
    {
        if(!get_value()){
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

bool syntax::get_arr_init()
{
    _p->try_hint("arr_init");

    if(!_p->match(DATATYPE))
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

    if(!_p->match(RBRACKET)){
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

    if(!_p->match(IDENTIFIER))
    {
        _p->cancel_try();
        return false;
    }

    if(!_p->match(COLON))
    {
        _p->cancel_try();
        return false;
    }

    if(!get_arr_init())
    {
        _p->error_here("Expected valid array declaration");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

void syntax::get_top_level()
{
    if(get_vardecl()){}
    else{_p->error_here("Unrecognized top-level statement");}
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