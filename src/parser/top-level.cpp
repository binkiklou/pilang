#include "parser.hpp"

#include "syntax.hpp"

#include "../shared/print.hpp"

bool syntax::get_assign()
{
    HINT_START("assignement");

    if(!_p->match(IDENTIFIER))
    {
        CANCEL_EXIT;
    }

    if(!_p->match(COLON))
    {
        CANCEL_EXIT;
    }

    // expect

    if(!get_array_expr())
    {
        _p->error_here("Expected an array expression for assignement.");
        ERROR_EXIT;
    }

    MATCH_EXIT;
}

// {...}
bool syntax::get_block()
{
    HINT_START("block");

    if(!_p->match(LBRACE))
    {
        CANCEL_EXIT;
    }

    while(!_p->match(RBRACE) && !_p->is_errored())
    {
        get_statement();

        if(_p->is_errored()){
            _p->recover();
        }
    }
    
    MATCH_EXIT;
}

bool syntax::get_proc()
{
    HINT_START("proc");

    if(!_p->match(KW_PROC))
    {
        _p->cancel_try();
        return false;
    }

    _p->warning_here("The implementation of procedures is incomplete");

    if(!get_block()){
        _p->error_here("Block is expected after a proc");
        _p->cancel_try();
        return false;
    }

    MATCH_EXIT;
}

// entry block
bool syntax::get_entry()
{
    HINT_START("entry");

    if(!_p->match(KW_ENTRY)){
        _p->cancel_try();
        return false;
    }

    if(!get_block()){
        _p->error_here("Block is expected after an entry");
        _p->cancel_try();
        return false;
    }

    MATCH_EXIT;
}

void syntax::get_statement()
{
    if(get_vardecl()){}
    else if(get_assign()){}
    else if(get_proc_call()){}
    else{_p->error_line_remain("Unrecognized statement");}
}

void syntax::get_top_level()
{ 
    if(get_entry()){}
    else if(get_proc()){}
    else{_p->error_line("Unrecognized top-level statement");}
}

bool parser::parse()
{
    if(m_state != PARSER_OK)
    {
        return false;
    }

    print_verbose("Starting parse");

    // Setup infinite recursion detection(hacky)
    _visit_tracking = std::vector<unsigned int>(tokens->size());
    for(unsigned int t : _visit_tracking) {t=0;}

    syntax s(this);

    // Top-Level loop 
    int last_pos = 0;
    int loop_count = 0;
    
    // Error handling not yet implemented
    while(this->_pos < this->tokens->size() && (m_state == PARSER_STATE::PARSER_OK || m_state == PARSER_STATE::PARSER_RECOVERED) )
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

        if(is_errored())
        {
            recover();
        }
    }

    if(m_state != PARSER_STATE::PARSER_OK){
        return false;
    }

    return true;
}