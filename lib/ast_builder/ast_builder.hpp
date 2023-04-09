#pragma once

#include "parser/parser.hpp"
#include "ast/ast.hpp"

class search_result
{
    public:
    bool found = false; // is there is a result
    bool is_unique = true; // if there is only one result
    std::vector<parser_node*> results;
};

// No error recovery for ast_builder
enum ast_builder_state {
    AST_BUILDER_OK,
    AST_BUILDER_FAIL
};

class ast_builder
{
    public:
    ast_builder();

    bool build();

    parser_node* parse_tree;
    std::vector<token>* tokens;

    ast_root ast;

    private:
    ast_builder_state m_state;

    // utils functions

    // naming things frustrates me
    // returns the min and max token position in tokens of tree
    std::pair<unsigned int, unsigned int> _get_token_pos_minmax(hint_node*);

    ast_loc_single* _get_single_loc(token_node*);
    ast_loc_large* _get_large_loc(hint_node*);

    //  Returns true if a node is valid
    bool _is_valid(ast_node*);

    std::vector<token_node*>  _get_tokens(parser_node*);
    std::vector<hint_node*> _get_hints(parser_node*);

    search_result _find_token(TKN_TYPE, parser_node*);
    search_result _find_hint(const std::string&, parser_node*);

    // build functions

    // common
    ast_identifier build_identifier(token_node*);
    ast_typespec build_typespec(hint_node*);

    // statements
    ast_vdecl build_vdecl(hint_node*);


    ast_block_stmt build_block_stmt(hint_node*);

    // top-level statements
    ast_entry_stmt build_entry_stmt(hint_node*);
};