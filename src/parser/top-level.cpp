#include "parser.hpp"

#include "syntax.hpp"

#include "../shared/print.hpp"

bool syntax::get_assign()
{
    _p->try_hint("assignement");

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

    // expect

    if(!get_array_expr())
    {
        _p->error_here("Expected an array expression for assignement.");
        _p->cancel_try();
        return false;
    }

    _p->keep_hint();
    return true;
}

void syntax::get_top_level()
{
    if(get_vardecl()){}
    else if(get_assign()){}
    else{_p->error_line_remain("Unrecognized top-level statement");}
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