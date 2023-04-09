#include "ast/stmt.hpp"

// --- ast_stmt ---

ast_stmt::ast_stmt()
{
    this->m_type = INVALID_STMT;
}

ast_vdecl::ast_vdecl()
{
    this->m_type = VAR_DECL_STMT;
}