#include "parser.hpp"

#include "../shared/print.hpp"

bool parser::parse()
{
    if(m_state != PARSER_OK)
    {
        return false;
    }

    print_verbose("Starting parse");

    // Top-Level loop 
    int last_pos = 0;
    int loop_count = 0;
    
    // Error handling not yet implemented
    while(this->_pos < this->tokens->size() && m_state == PARSER_STATE::PARSER_OK)
    {
        // Loop detection
        if(this->_pos == last_pos){
            loop_count++;
        }
        if(loop_count >= 3){
            this->m_state = PARSER_STATE::PARSER_UNRECOVERABLE;
            print_error("Parser infinite loop, exiting.");
            return false;
        }
    }

    return true;
}