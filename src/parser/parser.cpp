#include "parser.hpp"

#include "../shared/print.hpp"

#include <memory>

// --- NODES ---
parser_node::parser_node()
{
    this->m_type = PNODE_TYPE::EMPTY_NODE;
}

parser_node::~parser_node()
{
    print_verbose("Deleting base parser_node");
    for(parser_node* child : this->m_children)
    {
        delete child;
    }
}

std::string parser_node::get_node_str()
{
    return colored("[EMPTY]", BLUE);
}

hint_node::hint_node()
{
    this->m_type = PNODE_TYPE::HINT_NODE;
}

std::string hint_node::get_node_str()
{
    return "[HINT](" + this->m_hint + ")";
}

token_node::token_node()
{
    this->m_type = PNODE_TYPE::TOKEN_NODE;
}

std::string token_node::get_node_str()
{
    return colored("[TOKEN]", GREEN) + "("+tkn_type_as_string(m_token.m_type)+":"+m_token.m_word.data+")";
}

error_node::error_node()
{
    this->m_type = PNODE_TYPE::ERROR_NODE;
}

std::string error_node::get_node_str()
{
    return colored("[ERROR]",RED);
}

// --- PARSER ---
parser::parser()
{
    this->m_state = PARSER_OK;
    this->_node_ptr = &(this->m_root);
    this->_base_node_ptr = &(this->m_root);
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

    if(peek_now().m_type == t)
    {
        _pos++;
        return true;
    }
    return false;
}

bool parser::expect(const TKN_TYPE& t)
{
    if(!parser::match(t))
    {
        error_here(
            "Unexpected token: "+tkn_type_as_string(peek_now().m_type)+
            "expected: "+tkn_type_as_string(t)+"."
            );
        return false;
    }
    return true;
}

void parser::error_here(const std::string& msg)
{
    diagnostic d;
    d.m_type = diagnostic_type::syntax_diag;
    d.m_level = diagnostic_level::error;
    d.msg = msg;
    d.m_show = true;
    d.m_vlcount = 0;
    d.has_cursor = true;
    d.m_clength = peek_now().m_word.data.length();
    d.m_loc = peek_now().m_word.loc;
    this->diagnostics.push_back(d);

    // Add error node to tree
    _add_error_node();
    
    // TODO: Recover
    this->m_state = PARSER_STATE::PARSER_UNRECOVERABLE;
}

void parser::try_hint(const std::string& h)
{
    hint_node* n = new hint_node;
    n->m_hint = h;
    _node_ptr = n;
    _try_nodes.push(n);
    _try_pos.push(_pos);
}

void parser::keep_hint()
{
    // If last hint, add to main tree
    if(_try_nodes.size() == 1)
    {
        print_verbose("Merging hint with main tree");
        _base_node_ptr->m_children.push_back(_try_nodes.top());
        _node_ptr = _base_node_ptr;
        _try_nodes.pop();
        _try_pos.pop();
        return;
    }
    // Otherwise add to the hint before
    hint_node* ptr  = _try_nodes.top();
    _try_nodes.pop();
    _try_nodes.top()->m_children.push_back(ptr);
    _node_ptr = _try_nodes.top();
    _try_pos.pop();
}

void parser::cancel_try()
{
    _pos = _try_pos.top();
    _try_pos.pop();
    _node_ptr = _try_nodes.top();
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

void parser::_add_error_node()
{
    if(_node_ptr == nullptr){
        return;
    }

    error_node* node = new error_node;
    node->m_type = PNODE_TYPE::ERROR_NODE;
    _node_ptr->m_children.push_back(node);
}