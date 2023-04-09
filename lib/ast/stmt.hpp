#pragma once

#include "ast/common.hpp"

#include <vector>
#include <string>

enum STMT_TYPE {
    INVALID_STMT,
    DECL_STMT,
    DECLDEF_STMT,
    DEF_STMT
};

class ast_stmt : public ast_node
{
    public:
    ast_stmt();
    STMT_TYPE m_type;
};

class ast_decl : public ast_stmt
{
    public:
    ast_decl();
    typespec m_typespec;
    identifier m_ident;
};


class ast_block_stmt : public ast_node
{
    public:
    std::vector<ast_stmt*> m_stmts;
};