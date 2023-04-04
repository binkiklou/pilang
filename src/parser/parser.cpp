#include "parser.hpp"

#include "../shared/print.hpp"

// --- NODES ---
parser_node::parser_node()
{
    this->m_type = PNODE_TYPE::EMPTY_NODE;
}

hint_node::hint_node()
{
    this->m_type = PNODE_TYPE::HINT_NODE;
}

token_node::token_node()
{
    this->m_type = PNODE_TYPE::TOKEN_NODE;
}



// --- PARSER ---
parser::parser()
{
    this->m_state = PARSER_OK;
    this->_node_ptr = nullptr;
    this->_pos = 0;
}

token parser::peek_front()
{
    if(_pos >= tokens->size() - 1|| tokens == nullptr)
    {
        print_verbose("Cannot peek front");
    }
    return tokens->at(_pos + 1);
}

token parser::peek_now()
{
    if(_pos >= tokens->size() || tokens == nullptr)
    {
        print_verbose("Cannot peek now");
    }
    return tokens->at(_pos);
}

token parser::peek_back()
{ 
    if(_pos == 0 || tokens == nullptr)
    {
        print_verbose("Cannot peek back");
    }
    return tokens->at(_pos - 1);
}