#pragma once

#include "ast/common.hpp"
#include "ast/stmt.hpp"

// --- TOP LEVEL ---

enum TOP_LEVEL_STMT_TYPE {
    INVALID_TOP_STMT, // Should not happen
    IMPORT_STMT,
    ENTRY_STMT,
    PROC_DECL_STMT,
    FN_DECL_STMT
};

class ast_top_level_stmt : ast_node
{
    public:
    ast_top_level_stmt();
    TOP_LEVEL_STMT_TYPE m_type;
};

class ast_entry_stmt : public ast_top_level_stmt
{
    public:
    ast_entry_stmt();
    ast_block_stmt m_block;
};