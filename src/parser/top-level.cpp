#include "parser.hpp"

#include "syntax.hpp"

#include "../shared/print.hpp"

void syntax::get_top_level()
{
    if(get_vardecl()){}
    else{_p->error_line_remain("Unrecognized top-level statement");}
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