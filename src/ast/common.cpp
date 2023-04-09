#include "ast/common.hpp"

// --- AST LOCATION ---

ast_loc::ast_loc()
{
    m_type = LOC_INVALID;
}

ast_loc_single::ast_loc_single() {
    m_type = LOC_SINGLE;
}

ast_loc_large::ast_loc_large() {
    m_type  = LOC_LARGE;
}

// --- AST Node ---

ast_node::ast_node()
{
    m_loc = nullptr;
    m_state = AST_NODE_INVALID;
}

// --- Identifier ---
identifier::identifier()
{
    m_loc = new ast_loc_single();
}