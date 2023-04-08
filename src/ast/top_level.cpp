#include "ast/top_level.hpp"

// --- ast_top_level_stmt ---

ast_top_level_stmt::ast_top_level_stmt()
{
    this->m_type = INVALID_TOP_STMT;
}

// --- ast_entry ---

ast_entry_stmt::ast_entry_stmt()
{
    this->m_type = ENTRY_STMT;
}