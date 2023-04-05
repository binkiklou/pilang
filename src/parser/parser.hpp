#pragma once

#include "../lexer/lexer.hpp"

// NODE

enum PNODE_TYPE
{
    EMPTY_NODE,
    HINT_NODE,
    TOKEN_NODE,
    ERROR_NODE
};

class parser_node
{
    public:
    parser_node();
    
    // Used for dumping
    virtual std::string get_node_str();

    PNODE_TYPE m_type;
    std::vector<parser_node*> m_children;
};

class hint_node : public parser_node
{
    public:
    hint_node();
    
    virtual std::string get_node_str() override;
    
    std::string m_hint;
};

class token_node : public parser_node
{
    public:
    token_node();
    
    virtual std::string get_node_str() override;
    
    token m_token;
};

// Can be turned into a diagnostic by the parser
class error_node : public parser_node
{
    public:
    error_node();
    
    virtual std::string get_node_str() override;

    std::string msg;
    bool has_token;
    token err_tkn;
};

// PARSER

enum PARSER_STATE
{
    PARSER_OK, // Normal parsing
    PARSER_RECOVERED, // Parsing but won't finish the compile process
    PARSER_RECOVERING, // State directly after an error
    PARSER_UNRECOVERABLE // Terminate the parser
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

    // If the current tkn_type matches, advances position
    bool match(const TKN_TYPE&);

    std::vector<token>* tokens;
    parser_node root;

    private:
    PARSER_STATE m_state;
    parser_node* _node_ptr;
    int _pos;

    void _add_hint_node(const std::string&);
    void _add_token_node(const token&);
};