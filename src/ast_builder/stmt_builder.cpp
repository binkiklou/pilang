#include "ast_builder/ast_builder.hpp"

ast_block_stmt ast_builder::build_block_stmt(hint_node* pnode)
{
    ast_block_stmt block;

    //block.m_loc = _get_loc(pnode);

    for(hint_node* h : _get_hints(pnode)) {}

    return block;
}

ast_entry_stmt ast_builder::build_entry_stmt(hint_node* pnode)
{
    ast_entry_stmt stmt;

    stmt.m_loc = _get_large_loc(pnode);

    search_result block_search = _find_hint("block", pnode);
    
    if(!block_search.found)
    {
        stmt.m_state = AST_NODE_UNFOUND;
        return stmt;
    }

    stmt.m_block = build_block_stmt(static_cast<hint_node*>(block_search.results[0]));

    return stmt;
}