#pragma once

#include "parser/parser.hpp"
#include "ast/ast.hpp"

// No error recovery for ast_builder
enum ast_builder_state {
    AST_BUILDER_OK,
    AST_BUILDER_FAIL
};

class ast_builder
{
    public:
    bool build();

    parser_node* parse_tree;
    ast_root ast;

    private:
    ast_builder_state m_state;
};