#pragma once

#include "ast/common.hpp"

#include <vector>
#include <string>

enum STMT_TYPE {
    INVALID_STMT,
    VAR_DECL_STMT,
    VAR_DECLDEF_STMT,
    VAR_DEF_STMT
};

class ast_stmt : public ast_node
{
    public:
    ast_stmt();
    STMT_TYPE m_type;
};

class ast_vdecl : public ast_stmt
{
    public:
    ast_vdecl();
    ast_typespec m_typespec;
    ast_identifier m_ident;
};

class ast_block_stmt : public ast_node
{
    public:
    std::vector<ast_stmt*> m_stmts;
};