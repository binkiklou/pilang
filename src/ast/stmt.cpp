#include "ast/stmt.hpp"

// --- ast_stmt ---

ast_stmt::ast_stmt()
{
    this->m_type = INVALID_STMT;
}

ast_decl::ast_decl()
{
    this->m_type = DECL_STMT;
}