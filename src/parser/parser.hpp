#pragma once

#include "../lexer/lexer.hpp"

// NODE

enum PNODE_TYPE
{
    EMPTY_NODE,
    HINT_NODE, // Structural in the tree
    TOKEN_NODE
};

class parser_node
{
    public:
    parser_node();
    PNODE_TYPE m_type;
    std::vector<parser_node*> m_children;
};

class hint_node : parser_node
{
    public:
    hint_node();
    std::string m_hint;
};

class token_node : parser_node
{
    public:
    token_node();
    token m_token;
};

// PARSER

enum PARSER_STATE
{
    PARSER_OK,
    PARSER_RECOVERED,
    PARSER_RECOVERING, // State directly after an error
    PARSER_UNRECOVERABLE
};

class parser
{
    public:
    parser();

    bool parse();

    // Returns a copy of the next token
    token peek_back();

    // Returns the current token in the stream
    token peek_now();

    // Returns a copy the last token
    token peek_front();

    std::vector<token>* tokens;
    parser_node root;

    private:
    PARSER_STATE m_state;
    parser_node* _node_ptr;
    int _pos;

    void _add_hint_node();
    void _add_token_node();
};