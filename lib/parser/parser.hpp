#pragma once

#include <stack>

#include "lexer/lexer.hpp"

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

    // Probably creates memory leaks because of polymorphism
    ~parser_node();
    
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

// Currently not used but will add more precisions to the errors
class error_node : public parser_node
{
    public:
    error_node();
    
    virtual std::string get_node_str() override;
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

    // Match but create error if token is unexpected
    bool expect(const TKN_TYPE&);

    // Assume simple warning at current token
    void warning_here(const std::string&);

    // Assumes simple error at current token
    void error_here(const std::string&);

    // Assumes simple error is at current line
    void error_line(const std::string&);

    // Creates an error for the rest of the line
    void error_line_remain(const std::string&);

    // Saves the position, and create a potential hint
    // All nodes will be pushed to the potential hint
    // unless another hint is tried, if so, the tried hint
    // will be added to  the first hint.
    void try_hint(const std::string&);

    // Adds the hint to the final tree or the potential hint
    // that it is a part of.
    void keep_hint();

    // Go back to original postion and delete the hint
    void cancel_try();

    // Return true if in a state that stops the parser from parsing
    bool is_errored();

    // Tries to recover from error
    void recover();

    std::vector<token>* tokens;
    std::vector<diagnostic> diagnostics;
    parser_node m_root;

    private:
    PARSER_STATE m_state;

    // Number of times each token has been visited
    // Used  for recursion detection(very hacky)
    std::vector<unsigned int> _visit_tracking;

    parser_node* _node_ptr;
    parser_node* _base_node_ptr;

    int _pos;

    std::stack<hint_node*> _try_nodes;
    std::stack<unsigned int> _try_pos;

    void _add_hint_node(const std::string&);
    void _add_token_node(const token&);
    void _add_error_node();
};