#include "parser.hpp"

#include "../shared/print.hpp"

#include <memory>

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

bool parser::match(const TKN_TYPE& t)
{
    if(this->m_state != PARSER_STATE::PARSER_OK){
        return false;
    }

    if(peek_back().m_type == t)
    {
        _pos++;
        return true;
    }
    return false;
}

void parser::_add_hint_node(const std::string& h)
{
    if(_node_ptr == nullptr){
        return;
    }

    hint_node* node = new  hint_node;
    node->m_type = PNODE_TYPE::HINT_NODE;
    node->m_hint = h;
    _node_ptr->m_children.push_back(node);
}

void parser::_add_token_node(const token& t)
{
    if(_node_ptr == nullptr){
        return;
    }

    token_node* node = new token_node;
    node->m_type = PNODE_TYPE::TOKEN_NODE;
    node->m_token = t;
    _node_ptr->m_children.push_back(node);
}